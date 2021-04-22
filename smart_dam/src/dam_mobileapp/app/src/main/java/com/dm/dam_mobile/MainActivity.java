package com.dm.dam_mobile;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothDevice;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import btlib.BluetoothChannel;
import btlib.BluetoothUtils;
import btlib.ConnectToBluetoothServerTask;
import btlib.ConnectionTask;
import btlib.RealBluetoothChannel;
import btlib.exceptions.BluetoothDeviceNotFound;
import btlib.utils.C;

public class MainActivity extends AppCompatActivity {

    private static final int DAM_LEVEL_MAX = 100;
    private static final int DAM_LEVEL_MIN = 0;
    private static final int DAM_STEP = 20;
    private static final int DEFAULT_MANUAL = 80;

    private static final String BTN_DISABLED = "#5c5c5c";
    private static final String BTN_ENABLED = "#009688";

    /**
     *  jSON accepted keys
     *
     *  do -> dam opening
     *  l -> water level
     *  s -> state
     *  m -> control mode
     *
     */
    private static final List<String> JSON_KEYS = new ArrayList<>(Arrays.asList("DO", "L", "S", "M"));

    private static final int DAM_OPENING = 0;
    private static final int WATER_LEVEL = 1;
    private static final int DAM_STATE = 2;
    private static final int CONTROL_MODE = 3;

    private int damLevel;
    private float waterLevel;
    private RiverState riverState;
    private DamMode damMode;

    private TextView damLevelLabel;
    private TextView waterLevelLabel;

    private TextView stateView;
    private TextView damLevelView;
    private TextView waterLevelView;

    private Switch contextSwitch;
    private Button btnOpen;
    private Button btnClose;

    private BluetoothChannel btChannel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        //initialize Bluetooth connection
        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        if(btAdapter != null && !btAdapter.isEnabled()){
            startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), C.bluetooth.ENABLE_BT_REQUEST);
        }

        // default
        this.riverState = RiverState.CONNECTING;

        // connect to DC via bluetooth
        try {
            connectToBTServer();
        } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
            bluetoothDeviceNotFound.printStackTrace();
        }

        initUI();
        updateUI();
    }

    @Override
    protected void onStop() {
        super.onStop();
        this.btChannel.close();
    }

    /**
     * Opens the dam by decreasing the dam level by DAM_STEP
     * @param view
     */
    public void openDam(View view) {
        moveDam(1);
    }

    /**
     * Close the dam by raising the dam level by DAM_STEP
     * @param view
     */
    public void closeDam(View view){
        moveDam(-1);
    }

    /**
     * Initialize UI getting components
     */
    private void initUI(){
        //context set up
        this.contextSwitch = findViewById(R.id.contextSwitch);
        this.contextSwitch.setOnCheckedChangeListener((buttonView, isChecked) -> {

            if(isChecked){
                //update control mode
                damMode = DamMode.MANUAL;

                //enable usable buttons
                if(!operationOutOfBounds(-1)){
                    enableButton(btnClose);
                }
                if(!operationOutOfBounds(1)) {
                    enableButton(btnOpen);
                }
            }else{
                //update control mode
                damMode = DamMode.AUTOMATIC;

                //disable buttons
                disableButton(btnClose);
                disableButton(btnOpen);
            }

            // Pair the values in a map and send to DC
            Map<String, Object> mode = Map.of(JSON_KEYS.get(CONTROL_MODE), this.damMode.getCode());
            this.btChannel.sendMessage(setMessage(mode));

            Map<String, Object> damOpeningMessage = Map.of(JSON_KEYS.get(DAM_OPENING), DEFAULT_MANUAL);
            this.btChannel.sendMessage(setMessage(damOpeningMessage));

            //update text
            contextSwitch.setText(damMode.toString());
            contextSwitch.setTextColor(Color.parseColor(damMode.getColor()));
        });

        //buttons SetUp
        this.btnClose = findViewById(R.id.buttonClose);
        this.btnOpen = findViewById(R.id.buttonOpen);

        // labels
        this.damLevelLabel = findViewById(R.id.levelLabel);
        this.waterLevelLabel = findViewById(R.id.waterLabel);

        // state
        this.stateView = findViewById(R.id.state);

        // water
        this.waterLevelView = findViewById(R.id.water);

        // dam level
        this.damLevelView = findViewById(R.id.level);

        // disable buttons
        disableButton(this.btnOpen);
        disableButton(this.btnClose);
    }

    /**
     * Updates the UI by checking the alert level
     */
    private void updateUI(){
        //Normal State and over
        this.stateView.setText(this.riverState.toString());
        this.stateView.setTextColor(Color.parseColor(this.riverState.getColor()));

        // Pre Alarm and over
        if(this.riverState.getAlertLevel() > RiverState.NORMAL.getAlertLevel()){

            this.waterLevelLabel.setVisibility(View.VISIBLE);
            this.waterLevelView.setVisibility(View.VISIBLE);
            this.waterLevelView.setText(String.valueOf(this.waterLevel) + " m");

            // Alarm and over
            if(this.riverState.getAlertLevel() > RiverState.PRE_ALARM.getAlertLevel()){

                this.damLevelLabel.setVisibility(View.VISIBLE);
                this.damLevelView.setVisibility(View.VISIBLE);
                this.contextSwitch.setVisibility(View.VISIBLE);
                this.btnClose.setVisibility(View.VISIBLE);
                this.btnOpen.setVisibility(View.VISIBLE);

                this.damLevelView.setText(String.valueOf(this.damLevel));
                this.contextSwitch.setText(this.damMode.toString());
                this.contextSwitch.setTextColor(Color.parseColor(this.damMode.getColor()));

                if(damMode.getCode() > 0){
                    if(this.damLevel == DAM_LEVEL_MIN){
                        disableButton(this.btnOpen);
                    } else if(!this.btnOpen.isEnabled()){
                        enableButton(this.btnOpen);
                    }
                    if(this.damLevel == DAM_LEVEL_MAX){
                        disableButton(this.btnClose);
                    } else if(!this.btnClose.isEnabled()){
                        enableButton(this.btnClose);
                    }
                }else{
                    this.contextSwitch.setChecked(false);
                    disableButton(this.btnOpen);
                    disableButton(this.btnClose);
                }
            }else{
                //hide the rest
                this.damLevelLabel.setVisibility(View.GONE);
                this.damLevelView.setVisibility(View.GONE);

                this.btnClose.setVisibility(View.GONE);
                this.btnOpen.setVisibility(View.GONE);
                this.contextSwitch.setVisibility(View.GONE);
            }

        }else{
            // hide the rest
            this.waterLevelLabel.setVisibility(View.GONE);
            this.damLevelLabel.setVisibility(View.GONE);
            this.waterLevelView.setVisibility(View.GONE);
            this.damLevelView.setVisibility(View.GONE);

            this.btnClose.setVisibility(View.GONE);
            this.btnOpen.setVisibility(View.GONE);
            this.contextSwitch.setVisibility(View.GONE);
        }
    }

    /**
     * Moves the dam level in the direction of the verse by DAM_STEP
     * @param verse
     * The verse of the opration (-1 close, 1:open).
     */
    private void moveDam(final int verse) {
        if(!operationOutOfBounds(verse)) {
            // update dam level
            this.damLevel = this.damLevel + DAM_STEP * verse;

            // pair the values in a map
            Map<String, Object> message = new HashMap<>();
            message.put(JSON_KEYS.get(DAM_OPENING), this.damLevel);

            // send message to DC
            this.btChannel.sendMessage(setMessage(message));

            // update buttons
            Button pressedButton;
            Button otherButton;
            if(verse > 0){
                // dam was closed
                pressedButton = this.btnClose;
                otherButton = this.btnOpen;
            }else{
                // dam was opened
                pressedButton = this.btnOpen;
                otherButton = this.btnClose;
            }

            if(operationOutOfBounds(verse)) {
                //disable button
                disableButton(pressedButton);
            }
            //enable other button
            enableButton(otherButton);

            //update view
            this.damLevelView.setText(String.valueOf(this.damLevel));
        }
    }

    /**
     * Checks if an operation on the dam is possible.
     * @param verse
     * The verse of the opration (1: close, -1:open).
     * @return
     * Returns True if an operation is NOT POSSIBLE.
     */
    private Boolean operationOutOfBounds(final int verse){
        return (this.damLevel + verse > DAM_LEVEL_MAX || this.damLevel + verse < DAM_LEVEL_MIN);
    }

    /**
     * Enables a button.
     * @param btn
     * The button to enable.
     */
    private void disableButton(Button btn){
        btn.setBackgroundColor(Color.parseColor(BTN_DISABLED));
        btn.setEnabled(false);
    }

    /**
     * Disables a button.
     * @param btn
     * The button to disable.
     */
    private void enableButton(Button btn){
        btn.setBackgroundColor(Color.parseColor(BTN_ENABLED));
        btn.setEnabled(true);
    }

    /**
     * From Map<\String, Object> entries formats a string as a JSON.
     * @param values
     * The Key, Value entries to put in the JSON.
     * @return
     * Returns a String formatted as JSON.
     */
    private String setMessage(final Map<String, Object> values){
        String message = "";

        try {
            JSONObject json = new JSONObject();
            for(final String key : values.keySet()) {
                json.put(key, values.get(key));
            }
            message = json.toString();
        } catch (JSONException exception){
            exception.printStackTrace();
        }

        return message;
    }

    /**
     * Sets connection with bluetooth device.
     * @throws BluetoothDeviceNotFound
     */
    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils.getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);

        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
        //final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {

                btChannel = channel;
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(final String receivedMessage) {
                        // parse the json message
                        try {
                            JSONObject jsonMessage = new JSONObject(receivedMessage);

                            // check all accepted keys and update the global fields
                            for (final String key : JSON_KEYS){
                                if(jsonMessage.has(key)) {
                                    updateValueFromKey(key, String.valueOf(jsonMessage.get(key)));
                                }
                            }

                        } catch (JSONException exception){
                            exception.printStackTrace();
                        }
                        // update UI
                        updateUI();
                    }
                    @Override
                    public void onMessageSent(String sentMessage) {}
                });
            }

            @Override
            public void onConnectionCanceled() {
               riverState = RiverState.CONNECTING;
               updateUI();
            }
        }).execute();
    }

    /**
     * Updates the value of a parameter based on the key given.
     * @param key
     * The parameter to update.
     */
    private void updateValueFromKey(final String key, final String value){
        switch (key){
            case "M":
                switch (Integer.parseInt(value)) {
                    case 0:
                        this.damMode = DamMode.AUTOMATIC;
                        break;
                    case 1:
                        this.damMode = DamMode.MANUAL;
                        break;
                    default:break;
                }
                break;
            case "S":
                switch (Integer.parseInt(value)) {
                    case 0:
                        this.riverState = RiverState.NORMAL;
                        break;
                    case 1:
                        this.riverState = RiverState.PRE_ALARM;
                        break;
                    case 2:
                        this.riverState = RiverState.ALARM;
                        break;
                    default:
                        this.riverState = RiverState.CONNECTING;
                        break;
                }
                break;
            case "L":
                this.waterLevel = Float.parseFloat(value);
                break;
            case "DO":
                this.damLevel = Integer.parseInt(value);
                break;
            default:break;
        }
    }
}
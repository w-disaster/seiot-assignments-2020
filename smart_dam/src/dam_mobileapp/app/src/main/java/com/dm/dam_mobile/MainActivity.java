package com.dm.dam_mobile;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import android.bluetooth.BluetoothAdapter;

import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import btlib.BluetoothChannel;
import btlib.utils.C;


public class MainActivity extends AppCompatActivity {

    private static final int DAM_LEVEL_MAX = 100;
    private static final int DAM_LEVEL_MIN = 0;
    private static final int DAM_STEP = 20;
    private static final String BTN_DISABLED = "#5c5c5c";
    private static final String BTN_ENABLED = "#009688";

    private int damLevel;
    private int waterLevel;
    private DamState damState;
    private ControlMode context;

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

        /*!COMMENT FOR TESTING UI
        //initialize Bluetooth connection
        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        if(btAdapter != null && !btAdapter.isEnabled()){
            startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), C.bluetooth.ENABLE_BT_REQUEST);
        }
        */

        //later the dam state will be received by DS
        this.damState = DamState.CONNECTING;
        //level will also be received by DS
        this.damLevel = DAM_LEVEL_MAX;
        //water level will also be received by DS
        this.waterLevel = 50;

        this.context = ControlMode.AUTOMATIC;

        initUI();
        updateUI();
    }

    @Override
    protected void onStart() {
        super.onStart();

        //later the dam state will be received by DS
        this.damState = DamState.ALARM;

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
        moveDam(-1);
    }

    /**
     * Close the dam by raising the dam level by DAM_STEP
     * @param view
     */
    public void closeDam(View view){
        moveDam(1);
    }

    /**
     * Initialize UI getting components
     */
    private void initUI(){
        //context set up
        this.contextSwitch = findViewById(R.id.contextSwitch);
        this.contextSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                //TODO: send request to DS to context witch

                if(isChecked){
                    //update control mode
                    context = ControlMode.MANUAL;

                    //enable usable buttons
                    if(!operationOutOfBounds(1)){
                        enableButton(btnClose);
                    }
                    if(!operationOutOfBounds(-1)) {
                        enableButton(btnOpen);
                    }
                }else{
                    //update control mode
                    context = ControlMode.AUTOMATIC;

                    //disable buttons
                    disableButton(btnClose);
                    disableButton(btnOpen);
                }
                //update text
                contextSwitch.setText(context.toString());
                contextSwitch.setTextColor(Color.parseColor(context.getColor()));
            }
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
        this.stateView.setText(this.damState.toString());
        this.stateView.setTextColor(Color.parseColor(this.damState.getColor()));

        // Pre Alarm and over
        if(this.damState.getAlertLevel() > DamState.NORMAL.getAlertLevel()){

            this.waterLevelLabel.setVisibility(View.VISIBLE);
            this.waterLevelView.setVisibility(View.VISIBLE);
            this.waterLevelView.setText(String.valueOf(this.waterLevel));

            // Alarm and over
            if(this.damState.getAlertLevel() > DamState.PRE_ALARM.getAlertLevel()){

                this.damLevelLabel.setVisibility(View.VISIBLE);
                this.damLevelView.setVisibility(View.VISIBLE);
                this.contextSwitch.setVisibility(View.VISIBLE);
                this.btnClose.setVisibility(View.VISIBLE);
                this.btnOpen.setVisibility(View.VISIBLE);

                this.damLevelView.setText(String.valueOf(this.damLevel));
                this.contextSwitch.setText(this.context.toString());
                this.contextSwitch.setTextColor(Color.parseColor(this.context.getColor()));

                if(context.getCode() > 0){
                    if(this.damLevel == DAM_LEVEL_MIN){
                        disableButton(this.btnOpen);
                    }
                    if(this.damLevel == DAM_LEVEL_MAX){
                        disableButton(this.btnClose);
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
     * The verse of the opration (1: close, -1:open).
     */
    private void moveDam(final int verse) {
        if(!operationOutOfBounds(verse)) {
            // update dam level
            this.damLevel = this.damLevel + DAM_STEP * verse;

            // pair the values in a map
            Map<String, Object> message = new HashMap<>();
            message.put("level", this.damLevel);

            /*!COMMENT FOR TESTING UI
            // send message to DC
            this.btChannel.sendMessage(setMessage(message));
            */

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
}
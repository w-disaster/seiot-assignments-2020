package com.dm.dam_mobile;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;

import android.bluetooth.BluetoothAdapter;

import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;

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
    private static final String WATER_COLOR = "#0080ff";

    private int damLevel;
    private int waterLevel;
    private DamState damState;
    private ControlMode context;

    private TextView damLevelLabel;
    private TextView waterLevelLabel;

    private TextView stateView;
    private TextView damLevelView;
    private TextView waterLevelView;

    private Switch contextSwicth;
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
    protected void onPause() {
        super.onPause();
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
    @RequiresApi(api = Build.VERSION_CODES.R)
    public void openDam(View view) {
        moveDam(-1);
    }

    /**
     * Close the dam by raising the dam level by DAM_STEP
     * @param view
     */
    @RequiresApi(api = Build.VERSION_CODES.R)
    public void closeDam(View view){
        moveDam(1);
    }

    /**
     * Changes the control mode from MANUAL to AUTOMATIC and vice versa
     * @param view
     */
    public void switchControlMode(View view){
        //TODO: send request to DS to context witch

        if(this.contextSwicth.isChecked()){

            this.context = ControlMode.MANUAL;

            if(!operationOutOfBounds(1)){
                enableButton(btnClose);
            }
            if(!operationOutOfBounds(-1)) {
                enableButton(btnOpen);
            }
        }else{
            this.context = ControlMode.AUTOMATIC;
            disableButton(btnClose);
            disableButton(btnOpen);
        }

        this.contextSwicth.setText(this.context.toString());
    }

    /**
     * Initialize UI getting components
     */
    private void initUI(){
        //context set up
        this.contextSwicth = (Switch)findViewById(R.id.contextSwitch);

        //buttons SetUp
        this.btnClose = (Button)findViewById(R.id.buttonClose);
        this.btnOpen = (Button)findViewById(R.id.buttonOpen);

        // labels
        this.damLevelLabel = (TextView)findViewById(R.id.levelLabel);
        this.waterLevelLabel = (TextView)findViewById(R.id.waterLabel);

        // state
        this.stateView = (TextView)findViewById(R.id.state);

        // water
        this.waterLevelView = (TextView)findViewById(R.id.water);
        this.stateView.setTextColor(Color.parseColor(WATER_COLOR));

        // dam level
        this.damLevelView = (TextView)findViewById(R.id.level);
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

            this.waterLevelView.setVisibility(View.VISIBLE);
            this.waterLevelView.setText(this.waterLevel);

            // Alarm and over
            if(this.damState.getAlertLevel() > DamState.PRE_ALARM.getAlertLevel()){

                this.damLevelView.setVisibility(View.VISIBLE);
                this.contextSwicth.setVisibility(View.VISIBLE);
                this.btnClose.setVisibility(View.VISIBLE);
                this.btnOpen.setVisibility(View.VISIBLE);

                this.damLevelView.setText(this.damLevel);
                this.contextSwicth.setText(this.context.toString());
                this.contextSwicth.setTextColor(Color.parseColor(this.context.getColor()));

                if(context.getCode() > 0){
                    if(this.damLevel == DAM_LEVEL_MIN){
                        disableButton(this.btnOpen);
                    }

                    if(this.damLevel == DAM_LEVEL_MAX){
                        disableButton(this.btnClose);
                    }
                }else{
                    disableButton(this.btnOpen);
                    disableButton(this.btnClose);
                }
            }else{
                //hide the rest
                this.damLevelView.setVisibility(View.GONE);
                this.btnClose.setVisibility(View.GONE);
                this.btnOpen.setVisibility(View.GONE);
                this.contextSwicth.setVisibility(View.GONE);
            }

        }else{
            // hide the rest
            this.waterLevelView.setVisibility(View.GONE);
            this.damLevelView.setVisibility(View.GONE);
            this.btnClose.setVisibility(View.GONE);
            this.btnOpen.setVisibility(View.GONE);
            this.contextSwicth.setVisibility(View.GONE);
        }

    }

    /**
     * Moves the dam level in the direction of the verse by DAM_STEP
     * @param verse
     * The verse of the opration (1: close, -1:open).
     */
    @RequiresApi(api = Build.VERSION_CODES.R)
    private void moveDam(final int verse) {
        if(!operationOutOfBounds(verse)) {
            // aggiorno il livello diga
            this.damLevel = this.damLevel + DAM_STEP * verse;

            // accoppio i valori
            Map<String, Object> message = Map.of("level", this.damLevel);

            // mando messaggio di muovere anche alla diga fisicamente
            this.btChannel.sendMessage(setMessage(message));

            // aggiorno i bottoni
            Button pressedButton;
            Button otherButton;
            if(verse > 0){
                //la diga è stata chiusa
                pressedButton = this.btnClose;
                otherButton = this.btnOpen;
            }else{
                // la diga è stata aperta
                pressedButton = this.btnOpen;
                otherButton = this.btnClose;
            }

            if(operationOutOfBounds(verse)) {
                //disable button
                disableButton(pressedButton);
                //enable other button
                enableButton(otherButton);
            }else{
                enableButton(otherButton);
            }

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
            System.out.println("Errore nel creare il JSON");
            exception.printStackTrace();
        }

        return message;
    }
}
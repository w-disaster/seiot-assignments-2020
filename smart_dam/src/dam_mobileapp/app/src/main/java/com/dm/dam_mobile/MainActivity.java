package com.dm.dam_mobile;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;

import android.bluetooth.BluetoothAdapter;

import android.content.Intent;
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

        //context set up
        this.contextSwicth = (Switch)findViewById(R.id.contextSwitch);

        //buttons SetUp
        this.btnClose = (Button)findViewById(R.id.buttonClose);
        this.btnOpen = (Button)findViewById(R.id.buttonOpen);

        if(this.damLevel == DAM_LEVEL_MIN){
            disableButton(this.btnOpen);
        }

        if(this.damLevel == DAM_LEVEL_MAX){
            disableButton(this.btnClose);
        }

        // labels
        this.damLevelLabel = (TextView)findViewById(R.id.levelLabel);
        this.waterLevelLabel = (TextView)findViewById(R.id.waterLabel);

        // show state
        this.stateView = (TextView)findViewById(R.id.state);
        this.stateView.setText(this.damState.toString());
        this.stateView.setTextColor(Color.parseColor(this.damState.getColor()));

        //hide all until its sent by DS
        this.waterLevelView.setVisibility(View.GONE);
        this.damLevelView.setVisibility(View.GONE);

        this.waterLevelLabel.setVisibility(View.GONE);
        this.damLevelLabel.setVisibility(View.GONE);

        this.btnClose.setVisibility(View.GONE);
        this.btnOpen.setVisibility(View.GONE);
        this.contextSwicth.setVisibility(View.GONE);
    }

    @Override
    protected void onStart() {
        super.onStart();
        //later the dam state will be received by DS
        this.damState = DamState.ALARM;

        //Normal State and over
        this.stateView.setText(this.damState.toString());
        this.stateView.setTextColor(Color.parseColor(this.damState.getColor()));

        // hide in case of refresh
        this.waterLevelView.setVisibility(View.GONE);
        this.damLevelView.setVisibility(View.GONE);
        this.btnClose.setVisibility(View.GONE);
        this.btnOpen.setVisibility(View.GONE);
        this.contextSwicth.setVisibility(View.GONE);

        // Pre Alarm and over
        if(this.damState.getAlertLevel() > DamState.NORMAL.getAlertLevel()){

            this.waterLevelView.setVisibility(View.VISIBLE);

            //hide in case of refresh
            this.damLevelView.setVisibility(View.GONE);
            this.btnClose.setVisibility(View.GONE);
            this.btnOpen.setVisibility(View.GONE);
            this.contextSwicth.setVisibility(View.GONE);

            // Alarm and over
            if(this.damState.getAlertLevel() > DamState.PRE_ALARM.getAlertLevel()){
                this.damLevelView.setVisibility(View.VISIBLE);

                this.contextSwicth.setVisibility(View.VISIBLE);
                this.btnClose.setVisibility(View.VISIBLE);
                this.btnOpen.setVisibility(View.VISIBLE);

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
            }

        }
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

    public void openDam(View view) {
        moveDam(-1);
    }

    public void closeDam(View view){
        moveDam(1);
    }

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

    private void moveDam(final int verse){
        if(!operationOutOfBounds(verse)) {
            String message = "";

            // aggiorno il livello diga
            this.damLevel = this.damLevel + DAM_STEP * verse;

            //TODO: make jSON and send it to move the thing

            // mando messaggio di muovere anche alla diga fisicamente
            this.btChannel.sendMessage(message);

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

            // mando al dam service il livello

            // mando per blutooth al controller

            //update view
            this.damLevelView.setText(String.valueOf(this.damLevel));
        }
    }

    private Boolean operationOutOfBounds(final int verse){
        return (this.damLevel + verse > DAM_LEVEL_MAX || this.damLevel + verse < DAM_LEVEL_MIN);
    }

    private void disableButton(Button btn){
        btn.setBackgroundColor(Color.parseColor(BTN_DISABLED));
        btn.setEnabled(false);
    }

    private void enableButton(Button btn){
        btn.setBackgroundColor(Color.parseColor(BTN_ENABLED));
        btn.setEnabled(true);
    }
}
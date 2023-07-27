package com.dm.dam_mobile;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.UUID;

import btlib.BluetoothChannel;
import btlib.BluetoothUtils;
import btlib.ConnectToBluetoothServerTask;
import btlib.ConnectionTask;
import btlib.RealBluetoothChannel;
import btlib.exceptions.BluetoothDeviceNotFound;
import btlib.utils.C;

/*
 * @author: Luca Fabri
 */

public class MainActivity extends AppCompatActivity {

    private RiverData riverdata;
    private BluetoothChannel btChannel;
    private TextView stateView;
    private TextView stateCircle;
    private TextView waterLevel;
    private TextView damOpening;
    private TextView manualOpening;
    private SeekBar manualSeekBar;
    private Switch damMode;
    private Button connectBtn;
    public static final int DEFAULT_DAM_OPENING = 50;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        if(btAdapter != null && !btAdapter.isEnabled()){
            startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), C.bluetooth.ENABLE_BT_REQUEST);
        }

        this.stateView = ((TextView) findViewById(R.id.state));
        this.stateCircle = ((TextView) findViewById(R.id.stateCircle));
        this.damOpening = ((TextView) findViewById(R.id.damOpening));
        this.damMode = ((Switch) findViewById(R.id.mode));
        this.waterLevel = ((TextView) findViewById(R.id.waterLevel));
        this.manualSeekBar = ((SeekBar) findViewById(R.id.manualSeekBar));
        this.manualOpening = ((TextView) findViewById(R.id.manualOpeningValue));
        this.connectBtn = ((Button) findViewById(R.id.connectBtn));
        initUI();
    }

    private void initUI() {
        /* Button listener */
        this.connectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    connectToBTServer();
                } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                    bluetoothDeviceNotFound.printStackTrace();
                }
            }
        });

        /* State */
        this.stateView.setText(State.DISCONNECTED.getText());

        /* Mode Switch */
        this.damMode.setEnabled(false);
        this.damMode.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                manualSeekBar.setEnabled(isChecked);
                DamMode mode = isChecked ? DamMode.MANUAL : DamMode.AUTO;
                damMode.setText(mode.toString());

                JSONObject json = new JSONObject();
                if(mode.equals(DamMode.AUTO)){
                    manualOpening.setText("");
                    try {
                        json.put("M", mode.ordinal());
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                } else {
                    manualSeekBar.setProgress(DEFAULT_DAM_OPENING);
                    manualOpening.setText(DEFAULT_DAM_OPENING + "%");
                    damOpening.setText(manualOpening.getText());
                    try {
                        json.put("D", DEFAULT_DAM_OPENING);
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }
                btChannel.sendMessage(json.toString());
            }
        });

        /* SeekBar */
        this.manualSeekBar.setEnabled(false);
        this.manualSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {}

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                manualOpening.setText(Integer.toString(progress) + "%");
                damOpening.setText(manualOpening.getText());

                /* Send dam opening to DC */
                JSONObject json = new JSONObject();
                try {
                    json.put("D", progress);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                btChannel.sendMessage(json.toString());
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();

        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode, @Nullable final Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK) {
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED) {
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils.getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) throws JSONException, InterruptedException {

                connectBtn.setText(String.format("Status : connected to DC (device %s)",
                        serverDevice.getName()));

                connectBtn.setEnabled(false);
                stateView.setText(State.CONNECTED.getText());

                btChannel = channel;
                JSONObject obj = new JSONObject();
                obj.put("getLastData",0);
                btChannel.sendMessage(obj.toString());

                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        try {
                            /* Create RiverData object from received json */
                            JSONObject json = new JSONObject(receivedMessage);
                            State state = State.values()[json.getInt("S")];
                            DamMode mode = DamMode.values()[json.getInt("M")];
                            int damOpening = json.getInt("DO");
                            if(state.equals(State.NORMAL)){
                                riverdata = new RiverData(state, mode, damOpening);
                            }else {
                                riverdata = new RiverData(state, mode, damOpening, (float)json.getDouble("L"));
                            }
                            /* Update the UI */
                            updateUI();
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {}
                });
            }

            @Override
            public void onConnectionCanceled() {
                connectBtn.setText(String.format("Status : unable to connect, device %s not found!",
                        C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME));
            }

            private void setAutoModeView(){
                /* Disable mode switch and seekbar */
                damMode.setEnabled(false);
                /* Empty text in manual opening value */
                manualOpening.setText("");
                manualSeekBar.setEnabled(false);
            }

            private void updateUI(){
                State state = riverdata.getState();
                /* Set the state */
                stateView.setText(state.getText());
                switch(state){
                    case NORMAL:
                        /* Change drawable to green one */
                        stateCircle.setCompoundDrawablesWithIntrinsicBounds(R.drawable.green_circle, 0, 0, 0);
                        setAutoModeView();
                        waterLevel.setText("");
                        break;
                    case PRE_ALARM:
                        /* Change drawable to orange one */
                        stateCircle.setCompoundDrawablesWithIntrinsicBounds(R.drawable.orange_circle, 0, 0, 0);
                        setAutoModeView();
                        break;
                    case ALARM:
                        /* Change drawable to red one */
                        stateCircle.setCompoundDrawablesWithIntrinsicBounds(R.drawable.red_circle, 0, 0, 0);
                        damMode.setEnabled(true);
                        break;
                }
                /* Set water level if state != NORMAL */
                if(!state.equals(State.NORMAL)){
                    waterLevel.setText(Float.toString(riverdata.getWaterLevel()) + "m");
                }
                /* Set dam opening */
                damOpening.setText(Integer.toString(riverdata.getDamOpening()) + "%");
                /* Set dam mode */
                damMode.setText(riverdata.getDamMode().toString());
                damMode.setChecked(riverdata.getDamMode().equals(DamMode.MANUAL));
            }
        }).execute();
    }
}

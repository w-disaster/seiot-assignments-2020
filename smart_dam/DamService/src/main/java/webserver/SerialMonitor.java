package webserver;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import io.vertx.core.Context;
import jssc.*;

public class SerialMonitor implements SerialPortEventListener {
	
	private SerialPort serialPort;
	private Environment environment;
	private StringBuffer currentMsg;

	public SerialMonitor(String portName, Environment environment) {
		super();
		this.currentMsg = new StringBuffer("");
		this.serialPort = new SerialPort(portName);
		this.environment = environment;
	}

	public void start() {
		try {
			this.serialPort.openPort();
			
		    this.serialPort.setParams(SerialPort.BAUDRATE_9600,
		                         SerialPort.DATABITS_8,
		                         SerialPort.STOPBITS_1,
		                         SerialPort.PARITY_NONE);

		    this.serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | 
		                                  SerialPort.FLOWCONTROL_RTSCTS_OUT);
		    this.serialPort.purgePort(SerialPort.PURGE_RXCLEAR | SerialPort.PURGE_TXCLEAR);
		    this.serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
		    
		}
		catch (SerialPortException ex) {
		    System.out.println("There are an error on writing string to port т: " + ex);
		}
	}

	public void sendMsg(String msg) {
		char[] array = (msg+"\n").toCharArray();
		byte[] bytes = new byte[array.length];
		for (int i = 0; i < array.length; i++){
			bytes[i] = (byte) array[i];
		}
		try {
			synchronized (serialPort) {
				serialPort.writeBytes(bytes);
			}
		} catch(Exception ex){
			ex.printStackTrace();
		}
	}
	
	
	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public synchronized void close() {
		try {
			if (this.serialPort != null) {
				this.serialPort.removeEventListener();
				this.serialPort.closePort();
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	/**
	 * Handle an event on the serial port. Read the data and print it.
	 */
	@Override
	public synchronized void serialEvent(SerialPortEvent event) {
        if(event.isRXCHAR()) {
            try {
            	String msg = this.serialPort.readString(event.getEventValue());
            	
            	msg = msg.replaceAll("\r", "");
        		
        		currentMsg.append(msg);
        		
        		boolean goAhead = true;
        		
    			while(goAhead) {
    				String msg2 = currentMsg.toString();
    				int index = msg2.indexOf("\n");
        			if (index >= 0) {
        				String json = msg2.substring(0, index);

        				/* We read the Json file with the mode requested by Dam Controller */
        				Gson gson = new Gson();
                        Map<String, String> jsonMap = gson.fromJson(json, Map.class);
                        
                        /* We get the Mode and we set it if it's possible */
                        String mode = jsonMap.get("Mode");
                        this.environment.setMode(Mode.valueOf(mode));
                        
                        /* We send back the current Mode of the Environment that serves as an ACK/NAK */
                        Map<String, Mode> map = new HashMap<>();
                        map.put("Mode", this.environment.getMode());
                        String sendJson = gson.toJson(map, Map.class);
        				
                        /* Send */
                        this.sendMsg(sendJson);
                        
        				currentMsg = new StringBuffer("");
        				if (index + 1 < msg2.length()) {
        					currentMsg.append(msg2.substring(index + 1)); 
        				} 
        			} else {
        				goAhead = false;
        			}
    			}
    			
    			
            }
            catch (SerialPortException ex) {
                System.out.println("Error in receiving string from COM-port: " + ex);
            }
        }
	}
	
	
	public SerialPort getSerialPort() {
		return this.serialPort;
	}

}
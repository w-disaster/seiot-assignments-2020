package controllers.serial;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import jssc.*;

public class SerialCommChannel implements CommChannel, SerialPortEventListener {
	
	private SerialPort serialPort;
	private BlockingQueue<String> queue;
	private StringBuffer currentMsg;

	public SerialCommChannel(String portName) {
		super();
		try {
			this.currentMsg = new StringBuffer("");
			this.serialPort = new SerialPort(portName);
			this.queue = new ArrayBlockingQueue<String>(200);
			this.serialPort.openPort();
			
		    this.serialPort.setParams(SerialPort.BAUDRATE_115200,
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

	@Override
	public synchronized void sendMsg(String msg) {
		
		char[] array = (msg + "\n").toCharArray();
		
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
	
	@Override
	public String receiveMsg() throws InterruptedException {
		return queue.take();
	}

	@Override
	public boolean isMsgAvailable() {
		return !queue.isEmpty();
	}
	
	@Override
	public SerialPort getSerialPort() {
		return this.serialPort;
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
        				
        				queue.put(msg2.substring(0, index));
                        
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
            } catch (InterruptedException e) {
				e.printStackTrace();
			}
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
	
}
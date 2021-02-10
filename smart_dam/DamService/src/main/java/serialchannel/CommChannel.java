package serialchannel;

import jssc.SerialPort;

// TODO: Auto-generated Javadoc
/**
 * Simple interface for an async msg communication channel.
 *
 */
public interface CommChannel {
	
	/**
	 * Send a message represented by a string (without new line).
	 * 
	 * Asynchronous model.
	 *
	 * @param msg the msg
	 */
	void sendMsg(String msg);
	
	/**
	 * To receive a message. 
	 * 
	 * Blocking behaviour.
	 *
	 * @return the string
	 * @throws InterruptedException the interrupted exception
	 */
	String receiveMsg() throws InterruptedException;

	/**
	 * To check if a message is available.
	 *
	 * @return true, if is msg available
	 */
	boolean isMsgAvailable();
	
	
	/**
	 * Gets the serial port.
	 *
	 * @return the serial port
	 */
	SerialPort getSerialPort();

}

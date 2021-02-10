package damservice;

import webserver.CommChannel;
import webserver.Environment;
import webserver.EnvironmentImpl;
import webserver.Mode;
import webserver.MsgController;
import webserver.MsgControllerImpl;
import webserver.SerialCommChannel;
import webserver.SerialCommChannelControllerRunnable;
import webserver.State;

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		/* Environment Model*/
		Environment environment = new EnvironmentImpl(Mode.AUTO, State.ALARM);
		
		/* Dam controller communication */
		CommChannel channel = new SerialCommChannel("/dev/ttyACM0");	
		
		/* Waiting for Arduino reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");	
		
		/* We start the thread in charge of managing communication channel with Dam Controller */
		Runnable serialController = new SerialCommChannelControllerRunnable(environment, channel);
		Thread serialControllerThread = new Thread(serialController);
		serialControllerThread.start();
		
		/* Message controller */
		Thread.sleep(6000);
		MsgController msgController = new MsgControllerImpl(environment, channel.getSerialPort());
		
	}

}

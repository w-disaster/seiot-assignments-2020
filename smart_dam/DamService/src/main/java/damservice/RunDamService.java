package damservice;

import webserver.Environment;
import webserver.EnvironmentImpl;
import webserver.Mode;
import webserver.MsgController;
import webserver.MsgControllerImpl;
import webserver.SerialMonitor;
import webserver.State;

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		/* Environment */
		Environment environment = new EnvironmentImpl(Mode.AUTO, State.NORMAL);
		
		SerialMonitor serialMonitor = new SerialMonitor("/dev/ttyACM0", environment);	
		serialMonitor.start();
		
		/* attesa necessaria per fare in modo che Arduino completi il reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");	
		
		serialMonitor.sendMsg("ready");
		
		/* Message controller */
		MsgController msgController = new MsgControllerImpl(environment, serialMonitor.getSerialPort());
		
		
		
	}

}

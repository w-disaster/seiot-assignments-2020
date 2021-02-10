package damservice;

import environment.Environment;
import environment.EnvironmentImpl;
import environment.Mode;
import environment.State;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.HttpServerResponse;
import serialchannel.CommChannel;
import serialchannel.SerialCommChannel;
import serialchannel.SerialCommChannelControllerRunnable;
import server.MsgController;
import server.MsgControllerImpl;
import server.MyHttpServerHandler;

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		
		/* Environment Model*/
		Environment environment = new EnvironmentImpl(Mode.AUTO, State.ALARM);
		
		/* Vertx Server */
		Vertx vertx = Vertx.vertx();
		HttpServer server = vertx.createHttpServer();
	
		server.requestHandler(new MyHttpServerHandler());
		server.listen(8080);
		
		/* Dam controller communication */
		CommChannel channel = new SerialCommChannel("/dev/ttyACM0");	
		
		/* Waiting for Arduino reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		
		
		/* We start the thread in charge of managing communication channel with Dam Controller */
		Runnable serialController = new SerialCommChannelControllerRunnable(environment, channel);
		Thread serialControllerThread = new Thread(serialController);
		serialControllerThread.start();
		
		/* Message controller */
		MsgController msgController = new MsgControllerImpl(environment, channel.getSerialPort());
		
		
		
		System.out.println("Dam Service ready");	
	}

}

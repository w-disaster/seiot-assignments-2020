package damservice;

import controllers.serial.CommChannel;
import controllers.serial.SerialCommChannel;
import controllers.serial.SerialCommChannelControllerRunnable;
import controllers.server.DBMSController;
import controllers.server.DBMSControllerImpl;
import controllers.server.HTTPServerController;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.HttpServerResponse;
import model.Model;
import model.ModelImpl;
import model.Mode;
import model.State;
import tests.MsgController;
import tests.MsgControllerImpl;
import tests.MyHttpServerHandler;

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		
		/* Environment Model*/
		Model model = new ModelImpl(Mode.AUTO, State.ALARM);
		
		/* DBMS controller */
		DBMSController dbmsController = new DBMSControllerImpl();
		
		/* Vertx Server */
		Vertx vertx = Vertx.vertx();
		HTTPServerController serverController = new HTTPServerController(8080, model, dbmsController);
		vertx.deployVerticle(serverController);
		
		/* Dam controller communication */
		CommChannel channel = new SerialCommChannel("/dev/ttyACM0");	
		
		/* Waiting for Arduino reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		
		
		/* We start the thread in charge of managing communication channel with Dam Controller */
		Runnable serialController = new SerialCommChannelControllerRunnable(model, channel);
		Thread serialControllerThread = new Thread(serialController);
		serialControllerThread.start();
		
		/* Message controller */
		MsgController msgController = new MsgControllerImpl(model, channel.getSerialPort());
		
		
		
		System.out.println("Dam Service ready");	
	}

}

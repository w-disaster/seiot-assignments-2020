package view;

import controllers.serial.CommChannel;
import controllers.serial.SerialCommChannel;
import controllers.serial.SerialCommChannelControllerRunnable;
import controllers.server.DBMSController;
import controllers.server.DBMSControllerImpl;
import controllers.server.HTTPServerController;
import io.vertx.core.Vertx;
import jssc.SerialPortList;
import model.Model;
import model.ModelImpl;
import model.Mode;
import model.State;

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		
		/* Environment Model*/
		Model model = new ModelImpl(Mode.AUTO, State.ALARM);
		
		/* DBMS controller */
		DBMSController dbmsController = new DBMSControllerImpl();
		
		/* Dam controller communication */
		final String[] ports = SerialPortList.getPortNames();
		CommChannel channel = new SerialCommChannel(ports[0]);
		
		/* Waiting for Arduino reboot */
		System.out.println("Waiting Dam Controller for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready");
		
		/* Vertx Server */
		Vertx vertx = Vertx.vertx();
		HTTPServerController serverController = new HTTPServerController(8080, model, dbmsController, channel);
		vertx.deployVerticle(serverController);
		
		/* We start the thread in charge of managing communication channel with Dam Controller */
		Runnable serialController = new SerialCommChannelControllerRunnable(model, channel);
		Thread serialControllerThread = new Thread(serialController);
		serialControllerThread.start();
	}

}

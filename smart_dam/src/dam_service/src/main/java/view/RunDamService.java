package view;

import controllers.dbms.DBMSController;
import controllers.dbms.DBMSControllerImpl;
import controllers.serial.CommChannel;
import controllers.serial.SerialCommChannel;
import controllers.serial.SerialCommChannelControllerRunnable;
import controllers.server.HTTPServerController;
import io.vertx.core.Vertx;
import io.vertx.core.VertxOptions;
import jssc.SerialPortList;
import model.Data;

/*
 * @author: Luca Fabri
 */

public class RunDamService {

	public static void main(String[] args) throws InterruptedException {
		/* Model: RiverData Data table */
		Data data = new Data();
		
		/* DBMS controller */
		DBMSController dbmsController = new DBMSControllerImpl();
		
		/* Dam Controller communication */
		final String[] ports = SerialPortList.getPortNames();
		CommChannel channel = new SerialCommChannel(ports[0]);
		
		/* Waiting for Arduino reboot */
		System.out.println("Waiting Dam Controller for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready");
		
		// Start the thread in charge of managing received messages from Dam Controller
		Runnable serialRunnable = new SerialCommChannelControllerRunnable(data, channel);
		Thread serialControllerThread = new Thread(serialRunnable);
		serialControllerThread.start();
		
		/* Vertx Server */
		VertxOptions options = new VertxOptions();
		options.setBlockedThreadCheckInterval(15000 * 1000);
		
		Vertx vertx = Vertx.vertx(options);
		HTTPServerController serverController = new HTTPServerController(8080, data, dbmsController, channel);
		vertx.deployVerticle(serverController);
	}

}

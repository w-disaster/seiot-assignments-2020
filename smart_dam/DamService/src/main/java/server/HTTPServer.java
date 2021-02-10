package server;

import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.HttpServerResponse;

public class HTTPServer {
	
	public static void main(String[] args) {
		Vertx vertx = Vertx.vertx();
		
		/* Server for ESP8266 */
		HttpServer server = vertx.createHttpServer();
	
		server.requestHandler(new MyHttpServerHandler());
	
		server.listen(8080);
		System.out.println("DS ready");
		
		
		/* Server for Dam Dashboard */
		
		
	}
}

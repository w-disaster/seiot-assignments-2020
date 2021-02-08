package webserver;

import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.HttpServerResponse;

public class HTTPServer {
	
	public static void main(String[] args) {
		Vertx vertx = Vertx.vertx();
		HttpServer server = vertx.createHttpServer();
	
		server.requestHandler(request -> {
			
			if(request.method().toString().toUpperCase().equals("GET")){
				
				// This handler gets called for each request that arrives on the server
				HttpServerResponse response = request.response();
				//response.putHeader("content-type", "text/html");
				
				// Write to the response and end it
				String file = System.getProperty("user.dir") + "/src/main/java/webserver" + request.path();
				System.out.println(file);
				response.sendFile(file);
				
			}
		});
	
		server.listen(8080);
		System.out.println("DS ready");
	}
}

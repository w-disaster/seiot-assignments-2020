package tests;

import io.vertx.core.Handler;
import io.vertx.core.http.HttpServerRequest;
import io.vertx.core.http.HttpServerResponse;

public class MyHttpServerHandler implements Handler<HttpServerRequest> {

	
	
	@Override
	public void handle(HttpServerRequest request) {
		if(request.method().toString().toUpperCase().equals("GET")){
			
			// This handler gets called for each request that arrives on the server
			HttpServerResponse response = request.response();
			//response.putHeader("content-type", "text/html");
			
			// Write to the response and end it
			String file = System.getProperty("user.dir") + "/src/main/java/webserver" + request.path();
			System.out.println(file);
			response.sendFile(file);
			
		}
	}

}

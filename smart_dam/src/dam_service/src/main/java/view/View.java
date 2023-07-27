package view;


import model.Pair;

public class View {
	
	private static final String VIEW_PATH = System.getProperty("user.dir") + "/../dam_dashboard";
	
	public static final Pair<Pair<String, String>, String> getFile(String requestPath) {
		Pair<String, String> header;
		
		/* Response header type */
		if(requestPath.contains("css/")) {
			header = new Pair<>("content-type", "text/css");
		} else if(requestPath.contains("js/")) {
			header = new Pair<>("content-type", "text/javascript");
		}else {
			header = new Pair<>("content-type", "text/html");
		}
		/* Return the header and the absolute path of the file */
		return new Pair<Pair<String, String>, String>(header, VIEW_PATH + requestPath);
	}

}

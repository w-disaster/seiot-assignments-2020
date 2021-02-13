package tests;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.file.Paths;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.common.io.Files;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

public class Test {
	public static void main(String[] args) throws IOException {
				
		/*Persona persona1 = new Persona("luca", "fabri");
		Persona persona2 = new Persona("giovanni", "biagini");
		
		Map<Integer, Persona> map = new HashMap<>();
		map.put(1, persona1);
		map.put(2, persona2);
		
		// create a writer
	    Writer writer = new FileWriter("data.json");

	    // convert map to JSON File
	    new Gson().toJson(map, writer);

	    // close the writer
	    writer.close();
	    
	    
	    
	    // create Gson instance
	    Gson gson = new Gson();

	    // create a reader
	    Reader reader = new FileReader("data.json");
	    
	    // convert JSON file to map
	    
	    map = gson.fromJson(reader, new TypeToken<Map<Integer, Persona>>() {}.getType());

	    // print map entries
	    for (Map.Entry<Integer, Persona> entry : map.entrySet()) {
	        System.out.println(entry.getKey() + " = [" + entry.getValue().getNome() + ", " + entry.getValue().getCognome() + "]");
	    }

	    // close reader
	    reader.close();
		
		Gson gson = new Gson();
		
		String nome = "luca";
		List<String> nomi = new ArrayList<>();
		nomi.add("luca");
		nomi.add("giovanni");
		String json = gson.toJson(nomi);
		System.out.println(json);
	    */
	    SimpleDateFormat sdf = new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss");
	    long time = System.currentTimeMillis();
		Timestamp t = new Timestamp(time);
		System.out.println(time);
	}
	
}

package controllers.server;

import java.sql.Connection;
import java.util.List;
import java.util.Map;

import model.Pair;

// TODO: Auto-generated Javadoc
/**
 * The Interface DBController.
 */
public interface DBMSController {

	boolean insertData(long timestamp, float waterLevel, String damMode, String state, int damOpening);
	
	Map<Integer, List<Pair<String, String>>> getDataFromTimestampOnwards(long timestamp);
	
	Map<String, String> getLastData();
	
	/**
	 * Gets the connection.
	 *
	 * @return the connection
	 */
	Connection getConnection();
	
}

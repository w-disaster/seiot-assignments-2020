package controllers.server;

import java.sql.Connection;
import java.util.Map;

// TODO: Auto-generated Javadoc
/**
 * The Interface DBController.
 */
public interface DBMSController {

	boolean insertData(long timestamp, float waterLevel, String damMode, String state, String damOpening);
	
	Map<String, String> getLastData();
	
	/**
	 * Gets the connection.
	 *
	 * @return the connection
	 */
	Connection getConnection();
	
}

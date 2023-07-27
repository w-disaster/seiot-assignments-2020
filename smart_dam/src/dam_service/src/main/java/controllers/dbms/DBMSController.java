package controllers.dbms;

import java.sql.Connection;
import java.util.List;
import java.util.Map;
import model.Pair;
import model.Data;

// TODO: Auto-generated Javadoc
/**
 * The Interface DBController.
 */
public interface DBMSController {
	
	boolean executeInsert(Data riverData);
	
	Map<Integer, List<Pair<String, String>>> getDataFromTimestampOnwards(long timestamp);
	
	Map<String, String> getLastData();
	
	/**
	 * Gets the connection.
	 *
	 * @return the connection
	 */
	Connection getConnection();
	
}

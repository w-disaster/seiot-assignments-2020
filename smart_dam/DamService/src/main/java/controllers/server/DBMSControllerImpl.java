package controllers.server;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.HashMap;
import java.util.Map;
import java.sql.Timestamp;

// TODO: Auto-generated Javadoc
/**
 * The Class DBControllerImpl.
 */
public class DBMSControllerImpl implements DBMSController {
	
	/** The connection. */
	private Connection connection;
	
	/** The db name. */
	String dbName = "RiverData";
	
	/**
	 * Instantiates a new DB controller impl.
	 */
	public DBMSControllerImpl() {
    	//MySQL locale
    	
       String driver = "com.mysql.cj.jdbc.Driver";
       String dbUri = "jdbc:mysql://localhost:3306/"+ this.dbName + 
    		   "?useUnicode=true&useJDBCCompliantTimezoneShift=true&useLegacyDatetimeCode=false&serverTimezone=UTC";
   	   String userName = "root";
   	   String password = "password";
        
       Connection connection = null;
        try {
            System.out.println("DataSource.getConnection() driver = " + driver);
            Class.forName(driver);
            System.out.println("DataSource.getConnection() dbUri = " + dbUri);
            connection = DriverManager.getConnection(dbUri, userName, password);
        }
        catch (ClassNotFoundException e) {
            new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
        }
        catch(SQLException e) {
            new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
        }
        this.connection = connection;
    }
	
	/**
	 * Execute insert.
	 *
	 * @param query the query
	 * @return true, if successful
	 */
	private boolean executeInsert(String query) {
		int result = 0;
		System.out.println(query);
		try {
			Statement statement = this.connection.createStatement();
			result = statement.executeUpdate(query);
		} catch (SQLException e) {
        	new Exception(e.getMessage());
            System.out.println("Errore"+ e.getMessage());
        }	
		if(result == 1) {
			return true;
		}
		return false;
	}

	@Override
	public boolean insertData(long timestamp, float waterLevel, String damMode, String state, int damOpening) {
		int result = 0;
		try {
			Statement statement = this.connection.createStatement();
			String timestampAsString = new Timestamp(timestamp).toString();
			/* MUST TEST */
			result = statement.executeUpdate("INSERT INTO Data VALUES (" + 
					"'" + timestampAsString + "', " +
					waterLevel + ", " +
					"'" + damMode + "', " +
					"'" + state + "', " +
					damOpening + ");");
		} catch (SQLException e) {
        	new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
        }	
		if(result == 1) {
			return true;
		}
		return false;
	}

	@Override
	public Map<String, String> getDataFromTimestampOnwards(long timestamp) {
		Map<String, String> lastRow = new HashMap<>();
		/* MUST TEST */
		String query = "SELECT * FROM Data  WHERE Timestamp >= '" + new Timestamp(timestamp).toString() + "'";
	    try (Statement stmt = this.connection.createStatement()) {
	      ResultSet rs = stmt.executeQuery(query);
	      while (rs.next()) {
	        String timestampAsString = rs.getString("Timestamp");
	        lastRow.put("Timestamp", timestampAsString);
	        
	        String waterLevel = rs.getString("Waterlevel");
	        lastRow.put("WaterLevel", waterLevel);
	        
	        String damMode = rs.getString("DamMode");
	        lastRow.put("DamMode", damMode);
	        
	        String state = rs.getString("State");
	        lastRow.put("State", state);
	        
	        String damOpening = rs.getString("DamOpening");
	        lastRow.put("DamOpening", damOpening);
	      }
	    } catch (SQLException e) {
	    	new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
	    }
		return lastRow;
	}

	@Override
	public Connection getConnection() {
		return this.connection;
	}
}

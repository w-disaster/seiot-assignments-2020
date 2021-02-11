package controllers.server;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import model.Pair;

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
	public Map<Integer, List<Pair<String, String>>> getDataFromTimestampOnwards(long timestamp) {
		Map<Integer, List<Pair<String, String>>> data = new HashMap<>();
		/* MUST TEST */
		String query = "SELECT * FROM Data  WHERE Timestamp >= '" + new Timestamp(timestamp).toString() + "'";
	    try (Statement stmt = this.connection.createStatement()) {
	      ResultSet rs = stmt.executeQuery(query);
	      System.out.println(rs);
	      while (rs.next()) {
	    	List<Pair<String, String>> row = new ArrayList<>();
	    	
	    	Pair<String, String> time = new Pair<>("Timestamp", rs.getString("Timestamp"));
	    	row.add(time);
	    	Pair<String, String> waterLevel = new Pair<>("WaterLevel", rs.getString("WaterLevel"));
	    	row.add(waterLevel);
	    	Pair<String, String> damMode = new Pair<>("DamMode", rs.getString("DamMode"));
	    	row.add(damMode);
	    	Pair<String, String> state = new Pair<>("State", rs.getString("State"));
	    	row.add(state);
	    	Pair<String, String> damOpening = new Pair<>("DamOpening", rs.getString("DamOpening"));
	    	row.add(damOpening);
	       
	    	data.put(rs.getRow(), row);
	      }
	    } catch (SQLException e) {
	    	new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
	    }
		return data;
	}

	@Override
	public Connection getConnection() {
		return this.connection;
	}
}

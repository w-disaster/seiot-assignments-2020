package controllers.dbms;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import model.Pair;
import model.Data;
import model.State;

import java.sql.Timestamp;

public class DBMSControllerImpl implements DBMSController {
	
	/** The connection. */
	private Connection connection;
	String dbName = "RiverData";
	int maxId;
	
	/**
	 * Instantiates a new DB controller impl.
	 */
	public DBMSControllerImpl() {    	
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
        this.maxId = getRiverDataMaxId();
    }
	
	private int getRiverDataMaxId() {
		int maxId = 0;
		String query = "SELECT MAX(Id) AS Id FROM Data"; 
	    try (Statement stmt = this.connection.createStatement()) {
	      ResultSet rs = stmt.executeQuery(query);
	      if(rs.next()) {
	    	maxId = rs.getInt("Id");
	      }
	    } catch (SQLException e) {
	    	new Exception(e.getMessage());
            System.out.println("Error " + e.getMessage());
	    }
		return maxId;
	}
	
	@Override
	public boolean executeInsert(Data riverData) {
		this.maxId += 1;
		if(riverData.getState().equals(State.NORMAL)) {
			return insertNormalStateRiverData(this.maxId, riverData.getTimestamp());
		}else {
			return insertRiverData(this.maxId, riverData.getTimestamp(), riverData.getWaterLevel(), riverData.getMode().toString(),
					riverData.getState().toString(), riverData.getDamOpening());
		}
	}

	private boolean insertRiverData(int id, long timestamp, float waterLevel, String damMode, String state, int damOpening) {
		int result = 0;
		try {
			Statement statement = this.connection.createStatement();
			String timestampAsString = new Timestamp(timestamp).toString();
			result = statement.executeUpdate("INSERT INTO Data VALUES (" + 
					id + ", " +
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
	
	private boolean insertNormalStateRiverData(int id, long timestamp) {
		int result = 0;
		try {
			Statement statement = this.connection.createStatement();
			String timestampAsString = new Timestamp(timestamp).toString();
			result = statement.executeUpdate("INSERT INTO Data (Id, Timestamp, DamMode, State, DamOpening) VALUES (" +
					id + ", " + 
					"'" + timestampAsString + "', " +
					"'" + "AUTO" + "', " +
					"'" + "NORMAL" + "', " +
					0 + ");");
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
		String query = "SELECT * FROM Data  WHERE Timestamp > '" + new Timestamp(timestamp).toString() + "'";
	    try (Statement stmt = this.connection.createStatement()) {
	      ResultSet rs = stmt.executeQuery(query);
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
	public Map<String, String> getLastData() {
		Map<String, String> lastRow = new HashMap<>();
		String query = "SELECT * FROM Data ORDER BY Timestamp DESC LIMIT 1";
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

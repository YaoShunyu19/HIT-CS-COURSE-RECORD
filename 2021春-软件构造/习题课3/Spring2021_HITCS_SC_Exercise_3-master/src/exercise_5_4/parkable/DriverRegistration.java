package exercise_5_4.parkable;

public class DriverRegistration {
	
	private String driver;
	
	public void registerDriver(String name) {
		this.driver = name;
	}
	
	public String getDriverInfo() {
		return driver;
	}
}

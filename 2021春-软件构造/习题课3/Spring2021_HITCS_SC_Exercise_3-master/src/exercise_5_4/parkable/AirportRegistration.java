package exercise_5_4.parkable;

import java.util.Calendar;

public class AirportRegistration {
	
	private String airport;
	private Calendar date = Calendar.getInstance();
	
	public void registerAirport(String name, Calendar calendar) {
		this.airport = name;
		this.date.setTimeInMillis(calendar.getTimeInMillis());
	}
	
	public String getAirportInfo() {
		return airport;
	}
}

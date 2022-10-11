package exercise_5_4.parkable;

import java.util.Calendar;

public class Plane extends ConcreteParkable {

	private final AirportRegistration ar = new AirportRegistration();
	
	public Plane(String plate, int width) {
		super(plate, width);
		pricingUnit = 60;
		price = 1000;
	}

	public void registerAirport(String name, Calendar date) {
		ar.registerAirport(name, date);
	}
	
	public String getAirportInfo() {
		return ar.getAirportInfo();
	}
	
	@Override
	public String toString() {
		return "Plane " + super.toString();
	}
}

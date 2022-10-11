package exercise_5_4.parkable;

import exercise_5_4.factory.ParkableFactory;
import exercise_5_4.state.State;

public interface Parkable {

	public int getWidth();
	public String getPlate();
	
	public double getPricingUnit();
	public double getPrice();
	
	public State getState();
	public void setState(State state);
	
	public static Parkable create(String type, String plate, int width, String[] extraRegistrationInfo) throws Exception {
		return new ParkableFactory().create(type, plate, width, extraRegistrationInfo);
	}	
}

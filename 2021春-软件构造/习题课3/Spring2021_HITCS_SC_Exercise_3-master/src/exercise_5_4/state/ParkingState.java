package exercise_5_4.state;

public class ParkingState implements State {

	static State instance = new ParkingState();
	
	@Override
	public State parking() {
		throw new IllegalArgumentException();
	}

	@Override
	public State depart() {
		return OnroadState.instance;
	}
	
	public static State getInstance() {
		return instance;
	}
	
	@Override 
	public String toString() {
		return "In parking";
	}
}

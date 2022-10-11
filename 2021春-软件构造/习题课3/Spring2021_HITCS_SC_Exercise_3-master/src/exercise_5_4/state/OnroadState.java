package exercise_5_4.state;

public class OnroadState implements State {

	static State instance = new OnroadState();

	@Override
	public State parking() {
		return ParkingState.instance;
	}

	@Override
	public State depart() {
		throw new IllegalArgumentException();
	}

	public static State getInstance() {
		return instance;
	}
	
	@Override 
	public String toString() {
		return "On road";
	}
}

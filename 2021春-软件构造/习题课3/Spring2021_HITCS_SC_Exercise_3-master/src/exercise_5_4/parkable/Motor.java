package exercise_5_4.parkable;

public class Motor extends ConcreteParkable {

	private final DriverRegistration dr = new DriverRegistration();

	public Motor(String plate, int width) {
		super(plate, width);
		pricingUnit = 30;
		price = 5;
	}
	
	public void registerDriver(String name) {
		dr.registerDriver(name);
	}
	
	public String getDriverInfo() {
		return dr.getDriverInfo();
	}

	@Override
	public String toString() {
		return "Motor " + super.toString();
	}
}

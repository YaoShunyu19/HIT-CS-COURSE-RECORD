package exercise_5_4.parkable;

public class Car extends ConcreteParkable {

	private final DriverRegistration dr = new DriverRegistration();

	public Car(String plate, int width) {
		super(plate, width);
		pricingUnit = 30;
		price = 10;
	}

	public void registerDriver(String name) {
		dr.registerDriver(name);
	}

	public String getDriverInfo() {
		return dr.getDriverInfo();
	}

	@Override
	public String toString() {
		return "Car " + super.toString();
	}
}

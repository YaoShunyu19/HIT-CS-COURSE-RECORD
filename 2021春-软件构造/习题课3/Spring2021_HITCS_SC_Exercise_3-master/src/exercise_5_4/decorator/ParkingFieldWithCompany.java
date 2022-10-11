package exercise_5_4.decorator;

import java.util.Map;

import exercise_5_4.field.ParkingField;

public class ParkingFieldWithCompany extends ComplexParkingField implements ParkingField {

	private String company;

	private void checkRep() {
		assert company != null && company.length() > 0;
	}

	public ParkingFieldWithCompany(ParkingField pf, String company) {
		super(pf);
		this.company = company;

		checkRep();
	}

	@Override
	public void parking(String type, String plate, int width, int num, String[] extraRegistrationInfo)
			throws Exception {

		super.parking(type, plate, width, num, extraRegistrationInfo);
		System.out.println("-------" + plate + ", " + this.company + " welcome to parking");
	}

	@Override
	public void parking(String type, String plate, int width, String[] extraRegistrationInfo) {
		super.parking(type, plate, width, extraRegistrationInfo);
		System.out.println("-------" + plate + ", " + this.company + " welcome to parking");
	}

	@Override
	public double depart(String plate) throws Exception {
		double price = super.depart(plate);
		System.out.println("-------" + plate + ", " + this.company + " wish you a good drive");
		return price;
	}

	@Override
	public Map<Integer, String> status() {
		System.out.println("-------This is a " + this.company + " parking field");
		return super.status();
	}

	@Override
	public String toString() {
		return super.toString() + "This parking field is managed by " + this.company + "\n";
	}

}
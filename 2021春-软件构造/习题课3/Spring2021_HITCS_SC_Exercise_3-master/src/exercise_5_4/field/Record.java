package exercise_5_4.field;

import java.util.Calendar;

import exercise_5_4.parkable.Parkable;

public class Record {
	private final Parkable car;
	private final Lot lot;
	private final Calendar timeIn = Calendar.getInstance();
	private Calendar timeOut = null;
	private double fee = 0;

	public Record(Parkable car, Lot lot) {
		this.car = car;
		this.lot = lot;
	}

	public Calendar getTimeOut() {
		return timeOut;
	}

	public Lot getLot() {
		return lot;
	}

	public Parkable getCar() {
		return car;
	}

	public void setTimeOut() {
		this.timeOut = Calendar.getInstance();
	}

	public double calcFee() {
		long diffInMilis = this.timeOut.getTimeInMillis() - this.timeIn.getTimeInMillis();
		long diffInMinute = diffInMilis / (60 * 1000);

		//fee = ((int) (diffInMinute / 30) + 1) * 10;
		fee = ((int) (diffInMinute / car.getPricingUnit()) + 1) * car.getPrice();
		return fee;
	}
	
}

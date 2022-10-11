package exercise_3_6;

import java.util.Calendar;

public class Record {
	private final Car car;
	private final Lot lot;
	private final Calendar timeIn = Calendar.getInstance();
	private Calendar timeOut = null;
	private double fee = 0;

	public Record(Car car, Lot lot) {
		this.car = car;
		this.lot = lot;
	}

	public Calendar getTimeOut() {
		return timeOut;
	}

	public Lot getLot() {
		return lot;
	}

	public Car getCar() {
		return car;
	}
	
	public void setTimeOut() {
		this.timeOut = Calendar.getInstance();
	}
	
	public double calcFee() {
		fee = 2; 
		//TODO: calculate based on the difference between timeIn and timeOut
		return fee;
	}
}

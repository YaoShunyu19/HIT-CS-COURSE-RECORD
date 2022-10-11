package exercise_5_4.field;

public class Lot implements Comparable<Lot> {

	private int number;
	private int width;
	
	public Lot(int num, int width) {
		this.number = num;
		this.width = width;
	}
	
	public int getWidth() {
		return width;
	}
	
	public int getNumber() {
		return number;		
	}
	
	@Override
	public boolean equals(Object lot) {
		if (lot == null)
			return false;
		if (!(lot instanceof Lot))
			return false;
		Lot c = (Lot) lot;
		if (c.getNumber() == this.number)
			return true;
		
		return false;
	}
	
	@Override
	public int hashCode() {
		return number;
	}

	@Override
	public int compareTo(Lot lot) {
		return this.getNumber() - lot.getNumber();
	}
}

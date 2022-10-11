package exercise_3_6;

public class Lot {

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + number;
		result = prime * result + width;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Lot other = (Lot) obj;
		if (number != other.number)
			return false;
		if (width != other.width)
			return false;
		return true;
	}

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
}

package exercise_3_6;

public class Car {

	private String plate;
	private int width;
	
	public Car(String plate, int width) {
		this.plate = plate;
		this.width = width;
	}

	public int getWidth() {
		return width;
	}
	
	public String getPlate() {
		return plate;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((plate == null) ? 0 : plate.hashCode());
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
		Car other = (Car) obj;
		if (plate == null) {
			if (other.plate != null)
				return false;
		} else if (!plate.equals(other.plate))
			return false;
		if (width != other.width)
			return false;
		return true;
	}
	
}

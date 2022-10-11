package exercise_5_4.client;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import exercise_5_4.decorator.ParkingFieldWithCompany;
import exercise_5_4.field.ParkingField;

public class TextFileReader {

	public static List<ParkingField> constructParkingFields(String filename) throws Exception {

		List<ParkingField> fields = new LinkedList<>();
		Pattern regex = Pattern.compile(
				"PF::=<(([A-Za-z0-9]+\\s)*[A-Za-z0-9]+),([1-9][0-9]+|[5-9]),(|(([A-Za-z0-9]+\\s)*[A-Za-z0-9]+))>");
		Matcher m;

		try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
			
			String line = br.readLine();
			
			while (line != null) {
			
				m = regex.matcher(line.trim());
				
				if (m.matches()) {
					String name = m.group(1);
					int size = Integer.valueOf(m.group(3));
					String company = m.group(4);

					Map<Integer, Integer> lots = new HashMap<>();
					for (int i = 0; i < size; i++) 
						lots.put(i + 1, 20);
					
					ParkingField pf = ParkingField.create(lots);
					if (!company.equals(""))
						pf = new ParkingFieldWithCompany(pf, company);
					
					fields.add(pf);
				}
				else 
					System.out.println("\""+line+"\" is not legal syntax.");

				line = br.readLine();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			System.out.println("Cannot find " + filename);
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Cannot read " + filename);
		}
		return fields;
	}
}

package com.revature.repository;

import java.util.List;

import com.revature.employee.EmployeeInfo;



public interface EmployeeRepository {
	
	/**
	 * 
	 * @param Employee that will be persisted to database
	 */
	void save(EmployeeInfo employeeInfo);
	
	/**
	 * 
	 * @param id the primary key that is used to locate an Employee
	 */
	EmployeeInfo findById(int id);
	
	/**
	 * 
	 * @param First name of the Employee in the database
	 */
	EmployeeInfo findByFirstName(String firstName);
	
	/**
	 * 
	 * @param First name of the Employee in the database
	 */
	EmployeeInfo findByLastName(String LastName);
	
//	
//	/**
//	 * 
//	 * @param username
//	 * @param password
//	 * @return
//	 */
//	EmployeeInfo validate(String employee_username, String employee_password);
//	
	/**
	 * This method locates every single Employee in our database
	 */
	List<EmployeeInfo> findAll();
	
	/**
	 * 
	 * @param Employee that will be update in our database
	 */
	void update(EmployeeInfo employeeInfo);
	
	/**
	 * 
	 * @param Employee that will be deleted from the database
	 */
	void delete(EmployeeInfo employeeInfo);

	boolean validate(String employee_username, String employee_password);
	

}

package com.revature.repository;

import java.util.List;

import com.revature.employee.EmployeeTransactions;

public interface TransactionRepository {
	
	/**
	 * 
	 * @param Transactions that will be persisted to database
	 */
	void save(EmployeeTransactions employeeTransactions);
	
	/**
	 * 
	 * @param id the primary key that is used to locate a Transactions
	 */
	EmployeeTransactions findByTransId(int id);
	
	/**
	 * 
	 * @param Employee id that is used to locate a Transactions
	 */
	EmployeeTransactions findByEmpId(int id);
	
	/**
	 * 
	 * @param First name of the Employee associated to Transaction in the database
	 */
	EmployeeTransactions findByFirstName(String firstName);
	
	/**
	 * 
	 * @param Last name of the Employee associated to Transaction in the database
	 */
	EmployeeTransactions findByLastName(String LastName);
	
	/**
	 * This method locates every single Transaction in our database
	 */
	List<EmployeeTransactions> findAll();
	
	/**
	 * 
	 * @param Manager the ingredient that will be update in our database
	 */
	void update(EmployeeTransactions employeeTransactions);
	
	/**
	 * 
	 * @param Manager that will be deleted from the database
	 */
	void delete(EmployeeTransactions employeeTransactions);
	

}

package com.revature.repository;

import java.util.List;

import com.revature.manager.ManagerInfo;

public interface ManagerRepository {
	
	
	
	/**
	 * 
	 * @param Manager that will be persisted to database
	 */
	void save(ManagerInfo managerInfo);
	
	/**
	 * 
	 * @param id the primary key that is used to locate an Manager
	 */
	ManagerInfo findById(int id);
	
	/**
	 * 
	 * @param First name of the Manager in the database
	 */
	ManagerInfo findByFirstName(String firstName);
	
	/**
	 * 
	 * @param First name of the Manager in the database
	 */
	ManagerInfo findByLastName(String LastName);
	
	/**
	 * This method locates every single Manager in our database
	 */
	List<ManagerInfo> findAll();
	
	/**
	 * 
	 * @param Manager the ingredient that will be update in our database
	 */
	void update(ManagerInfo managerInfo);
	
	/**
	 * 
	 * @param Manager that will be deleted from the database
	 */
	void delete(ManagerInfo managerInfo);
	
}

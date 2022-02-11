package com.revature.service;

import java.util.List;

import com.revature.employee.EmployeeInfo;
import com.revature.repository.EmployeeRepository;
import com.revature.repository.EmployeeRepositoryImpl;

public class EmployeeService {
	
	private EmployeeRepository employeeRepositoryImpl;
	
	public EmployeeService() {
		this.employeeRepositoryImpl = new EmployeeRepositoryImpl();
	}

	public List<EmployeeInfo> findAll(){ 
		
		return this.employeeRepositoryImpl.findAll();
 }

	public void save(EmployeeInfo employeeInfo) {
		this.employeeRepositoryImpl.save(employeeInfo);
	}
	
	public EmployeeInfo findById(int id) {
		return this.employeeRepositoryImpl.findById(id);
	}
	
	public boolean validate(String employee_username, String employee_password) {
		
		return true;
	}

	public void validate(EmployeeInfo retrievedUser, EmployeeInfo retrievedUser2) {
		// TODO Auto-generated method stub
		
	}


	
}

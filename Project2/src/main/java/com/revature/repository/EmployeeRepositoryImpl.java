package com.revature.repository;


import java.util.List;

import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.Transaction;

import com.revature.employee.EmployeeInfo;
import com.revature.utility.HibernateSessionFactory;

public class EmployeeRepositoryImpl implements EmployeeRepository {

	@Override
	public void save(EmployeeInfo employeeInfo) {
		// TODO Auto-generated method stub
		
		Session session = null;
		Transaction transaction = null;
		
		try {
			session = HibernateSessionFactory.getSession();
			transaction = session.beginTransaction();
			session.save(employeeInfo);
			transaction.commit();
		}catch(HibernateException e) {
			transaction.rollback();
			e.printStackTrace();
		}finally {
			session.close();
		}
		
	}

	@Override
	public EmployeeInfo findById(int id) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public EmployeeInfo findByFirstName(String firstName) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public EmployeeInfo findByLastName(String LastName) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public List<EmployeeInfo> findAll() {
		// TODO Auto-generated method stub
		
		List<EmployeeInfo> employeeInfo = null;
		
	
		Session session = null;
	
		Transaction transaction = null;
		
		try {
			session = HibernateSessionFactory.getSession();
			transaction = session.beginTransaction();
	
			employeeInfo = session.createQuery("FROM EmployeeInfo", EmployeeInfo.class).getResultList();

			transaction.commit();
			
		}catch(HibernateException e) {
			
			transaction.rollback();
			e.printStackTrace();
		}
		
		return employeeInfo;
		
	}

	@Override
	public void update(EmployeeInfo employeeInfo) {
		// TODO Auto-generated method stub
		
		Session session = null;
		Transaction transaction = null;
		
		try {
			session = HibernateSessionFactory.getSession();
			transaction = session.beginTransaction();
			
			session.update(employeeInfo);
			
			transaction.commit();
			
		}catch(HibernateException e) {
			
			transaction.rollback();
			e.printStackTrace();
		}
		
		
	}

	@Override
	public void delete(EmployeeInfo employeeInfo) {
		// TODO Auto-generated method stub
		Session session = null;
		Transaction transaction = null;
		
		try {
			session = HibernateSessionFactory.getSession();
			transaction = session.beginTransaction();
			
			session.delete(employeeInfo);
			
			transaction.commit();
			
		}catch(HibernateException e) {
			
			transaction.rollback();
			e.printStackTrace();
		}
		
	}

	@Override
	public boolean validate(String employee_username, String employee_password) {
		// TODO Auto-generated method stub
		Session session = null;
		Transaction transaction= null;
		 Object user = null;
		
		try {
			session = HibernateSessionFactory.getSession();
			transaction = session.beginTransaction();
			user = (EmployeeInfo) session.createQuery("from employee_info ei where ei.employee_username = :employee_username")
					.uniqueResult();
			if(user != null && ((EmployeeInfo) user).getPassword().equals(employee_password)) {
			
				System.out.println("Welcome user");
				return true;
			}
		}catch(HibernateException e) {
			
			transaction.rollback();
			e.printStackTrace();
		}
		System.out.println("Invalid Entry, try again!");
		return false;
		
	}

}

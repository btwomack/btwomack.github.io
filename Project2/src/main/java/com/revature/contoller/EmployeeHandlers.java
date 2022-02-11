package com.revature.contoller;


import com.revature.employee.EmployeeInfo;
import com.revature.service.EmployeeService;

import io.javalin.http.Handler;

public class EmployeeHandlers {
	
	private EmployeeService employeeService;
	
	public EmployeeHandlers() {
		
		employeeService = new EmployeeService();
		
	}

	public Handler findAllEmployees = ctx -> {
		
		System.out.println("Find all Employees has been hit.");
		
		System.out.println(ctx.queryString());

		// To isolate a parameter:

		System.out.println(ctx.queryParam("minCost"));
		System.out.println(ctx.queryParam("maxCost"));
		
		ctx.json(employeeService.findAll());
	
	};
	
	

	public Handler saveEmployee = ctx -> {
		
		System.out.println(ctx.body());
		
		EmployeeInfo retrievedEmployee = ctx.bodyAsClass(EmployeeInfo.class);
		System.out.println(retrievedEmployee);
		// Save the ingredient
		employeeService.save(retrievedEmployee);
		
		
		ctx.status(201);
		
	};
	
	public Handler findEmployeeById = ctx -> {
		// Proof of Concept
		System.out.println(ctx.pathParam("id"));
		EmployeeInfo retrievedEmployee = employeeService.findById(Integer.parseInt(ctx.pathParam("id")));
		// Don't forget to write the ingredient to the response body as JSON.
		if (retrievedEmployee != null)
			ctx.json(retrievedEmployee);
		else
			ctx.res.getWriter().write("Sorry. No ingredients that match your search were found.");
};

}


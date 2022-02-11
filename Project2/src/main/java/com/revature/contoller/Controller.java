package com.revature.contoller;

import static io.javalin.apibuilder.ApiBuilder.get;
import static io.javalin.apibuilder.ApiBuilder.path;
import static io.javalin.apibuilder.ApiBuilder.post;


import io.javalin.Javalin;

public class Controller {

	
	private Javalin app;
	private EmployeeHandlers employeeHandler;
	
	
	public Controller(Javalin app) {
		this.app = app;
		this.employeeHandler = new EmployeeHandlers();
	}
	
	public void initEndpoints() {
		
		//EndpointGroup
		this.app.routes(() -> {
			
			path("/employee", () -> {
				
				 path("/all", () -> {
					 
					 get(this.employeeHandler.findAllEmployees);
				 });
				 
				 path("/new", () -> {
					 
					 post(this.employeeHandler.saveEmployee);
				 });
				 
				 path("/id/{id}", () -> {
					 
					 get(this.employeeHandler.findEmployeeById);
					 
				 });
			});
		});
	}
	
	
}

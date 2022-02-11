package com.revature;

import io.javalin.Javalin;


import com.revature.employee.EmployeeInfo;
import com.revature.service.EmployeeService;

//import javax.servlet.http.HttpSession;

//import com.revature.contoller.Controller;
import com.revature.utility.JavalinConfig;

public class Driver {
	
	public static void main(String[] args) {
		
		Javalin app = Javalin.create().start(7004);
		
		JavalinConfig config = new JavalinConfig(app).configureHttpMethodPreference().configureStaticResources(); //configureHttpMethodPreference() goes before configureStaticRescources()
		
		app.get("/user/auth", ctx -> {
			System.out.println(ctx.body());
//			
//			EmployeeInfo retrievedUser = ctx.bodyAsClass(EmployeeInfo.class);
//			EmployeeInfo retrievedUser2 = ctx.bodyAsClass(EmployeeInfo.class);
//			
//			EmployeeService employeeService = new EmployeeService();
//			employeeService.validate(retrievedUser, retrievedUser2);
			
			
			
		});
		
		
//		// Initializing the controller
//		Controller controller = new Controller(app);
//		controller.initEndpoints();
//		
//		app.before(ctx -> {
//			System.out.println("Ayyyye! Jenkins works.");
//		});
//		
//
//		app.after(ctx -> {
//			System.out.println("This happens after the request has made it to its designated handler.");
//		});
//		
//
//		app.error(404, ctx -> {
//			ctx.redirect("/404.html");
//		});
//
//		app.post("/session", ctx -> {
//	
//			ctx.req.getSession();
//		});
//
//		app.get("/locked-resource", ctx -> {
//			
//			
//			HttpSession session = ctx.req.getSession(false);
//			if(session == null) ctx.res.getWriter().write("Sorry you can't access this resource.");
//			else ctx.res.getWriter().write("Congrats. You're authenticated.");
//		});
//		
//	
//		app.get("/logout", ctx -> {
//			HttpSession session = ctx.req.getSession(false);
//			if(session != null) session.invalidate();
//		});
//
//		app.get("/hello-jenkins", ctx -> {
//			ctx.res.getWriter().write("ayeeeee jenkins works for real this time with set +e and correct path");
//		});

}
}
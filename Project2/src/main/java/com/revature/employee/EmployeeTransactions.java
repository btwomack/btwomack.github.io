package com.revature.employee;

import java.util.Objects;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.SequenceGenerator;
import javax.persistence.Table;


@Entity
@Table(name = "employee_transactions")
public class EmployeeTransactions {
	
	
	@Id
	@Column(name = "trans_id")
	@GeneratedValue(generator = "trans_id_seq", strategy = GenerationType.AUTO)
	@SequenceGenerator(allocationSize = 1, name = "trans_id_seq", sequenceName = "trans_id_seq")
	private int transId;
	@Column(name = "trans_title")
	private String transTitle;
	@Column(name = "trans_description")
	private String transDescription;
	@Column(name = "trans_amount")
	private float transAmount;
	@Column(name = "trans_approval")
	private boolean approval;
	@Column(name = "approval_description")
	private String approvalDescription;
	
	@ManyToOne
	private EmployeeInfo employeeId;

	public EmployeeTransactions() {
		super();
		// TODO Auto-generated constructor stub
	}

	public EmployeeTransactions(int transId, String transTitle, String transDescription, float transAmount,
			boolean approval, String approvalDescription, EmployeeInfo employeeId) {
		super();
		this.transId = transId;
		this.transTitle = transTitle;
		this.transDescription = transDescription;
		this.transAmount = transAmount;
		this.approval = approval;
		this.approvalDescription = approvalDescription;
		this.employeeId = employeeId;
	}

	public int getTransId() {
		return transId;
	}

	public void setTransId(int transId) {
		this.transId = transId;
	}

	public String getTransTitle() {
		return transTitle;
	}

	public void setTransTitle(String transTitle) {
		this.transTitle = transTitle;
	}

	public String getTransDescription() {
		return transDescription;
	}

	public void setTransDescription(String transDescription) {
		this.transDescription = transDescription;
	}

	public float getTransAmount() {
		return transAmount;
	}

	public void setTransAmount(float transAmount) {
		this.transAmount = transAmount;
	}

	public boolean isApproval() {
		return approval;
	}

	public void setApproval(boolean approval) {
		this.approval = approval;
	}

	public String getApprovalDescription() {
		return approvalDescription;
	}

	public void setApprovalDescription(String approvalDescription) {
		this.approvalDescription = approvalDescription;
	}

	public EmployeeInfo getEmployeeId() {
		return employeeId;
	}

	public void setEmployeeId(EmployeeInfo employeeId) {
		this.employeeId = employeeId;
	}

	@Override
	public int hashCode() {
		return Objects.hash(approval, approvalDescription, employeeId, transAmount, transDescription, transId,
				transTitle);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		EmployeeTransactions other = (EmployeeTransactions) obj;
		return approval == other.approval && Objects.equals(approvalDescription, other.approvalDescription)
				&& Objects.equals(employeeId, other.employeeId)
				&& Float.floatToIntBits(transAmount) == Float.floatToIntBits(other.transAmount)
				&& Objects.equals(transDescription, other.transDescription) && transId == other.transId
				&& Objects.equals(transTitle, other.transTitle);
	}

	@Override
	public String toString() {
		return "EmployeeTransactions [transId=" + transId + ", transTitle=" + transTitle + ", transDescription="
				+ transDescription + ", transAmount=" + transAmount + ", approval=" + approval
				+ ", approvalDescription=" + approvalDescription + ", employeeId=" + employeeId + "]";
	}
		
	
}
	
	
	
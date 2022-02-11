console.log('it works finally')

let button = document.getElementById('data-submission')


function sendCredentials() {
    let xhr = new XMLHttpRequest()

    let url = 'http://localhost:7004/user/auth'
    xhr.open('GET', url)
    
    let username = document.getElementById('username').value 
    let password = document.getElementById('password').value

    let user = {username:username, password:password} 
    console.log(user)

let json_string = JSON.stringify(user)

xhr.send(json_string)
}

button.addEventListener('click', sendCredentials)
console.log('invalid entry')
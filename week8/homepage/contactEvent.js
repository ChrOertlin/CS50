// EventListener for the submit button on the contact form
// to do add functions that handles textfield extraction
// and sending an email with text
document.addEventListener("DOMContentLoaded", onSubmitEvent);

function onSubmitEvent(){
    let subBtn = document.getElementById("contactButton");
    subBtn.addEventListener("click", function() {
        alert("the button has been clicked");
    });

}
// add a location listener to the page that changes navBar elements based on it
let pageLoc = window.location.href;
var navElems = document.querySelectorAll(".indexNav-link");
navElems.forEach(elem => {
    if (elem.href == pageLoc) {
        elem.classList.add("active");
    }
    else {
        elem.classList.remove("active");
    }
})

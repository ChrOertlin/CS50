// Script to automatically add navigation bar on top of page when called
fetch('nav.html')
.then(res => res.text())
.then(text => {
    console.log("executed");
    let oldelem = document.querySelector("script#replace_with_navbar");
    let newelem = document.createElement("div");
    newelem.innerHTML = text;
    oldelem.parentNode.replaceChild(newelem,oldelem);
})

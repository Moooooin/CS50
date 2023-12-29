let img1 = document.querySelector("#bild1");
let img2 = document.querySelector("#bild2");
let is1 = true;

setInterval(function() {
    if (is1) {
        img1.style.display = 'none';
        img2.style.display = 'flex';
        is1 = false;
    } else {
        img1.style.display = 'flex';
        img2.style.display = 'none';
        is1 = true;
    }
}, 5000);

// cs50 duck helped to write this code (The interval part)
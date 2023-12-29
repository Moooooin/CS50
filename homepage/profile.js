let textElement = document.querySelector("#change");
let originalText = 'Nationality : German';
let alternateText = 'Nationality : Brazilian';
let isOriginal = true;

setInterval(function() {
    if (isOriginal) {
        textElement.textContent = alternateText;
        isOriginal = false;
    } else {
        textElement.textContent = originalText;
        isOriginal = true;
    }
}, 2500);

// cs50 duck helped to write this code (The interval part)
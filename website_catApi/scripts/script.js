document.addEventListener('DOMContentLoaded', () => {
    fetchCatImage();
});

document.getElementById('fetch-cat-button').addEventListener('click', fetchCatImage);

function fetchCatImage() {
    const catContainer = document.getElementById('cat-container');
    catContainer.innerHTML = '';
    fetch('https://api.thecatapi.com/v1/images/search', {
        method: 'GET',
        headers: {
            'x-api-key': 'YOUR_API_KEY'
        }
    })
    .then(response => response.json())
    .then(data => {
        const imgElement = document.createElement('img');
        imgElement.src = data[0].url;
        imgElement.alt = 'A cute cat';
        catContainer.appendChild(imgElement);
    })
    .catch(error => {
        console.error('Error fetching cat image:', error);
    });
}

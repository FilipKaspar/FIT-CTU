const accountId = window.location.pathname.substring(window.location.pathname.lastIndexOf('/') + 1);
const prevURL = document.referrer.substring(window.location.pathname.lastIndexOf('/') + 1);

const selectElement = document.getElementById('account_employee');

let selectedEmployee = selectElement.options[0].value;
selectElement.addEventListener('change', function() {
    selectedEmployee = selectElement.value;
});

smazatBtn = document.getElementById('account_Smazat');
if(smazatBtn) {
    smazatBtn.onclick = function (event) {

        var result = confirm("Are you sure you want to delete this account?");

        if (!result) {
            event.preventDefault();
            return;
        }

        const apiUrl = `http://localhost:8080/api/account/${accountId}`;

        fetch(apiUrl, {
            method: 'DELETE',
            headers: {
                'Content-Type': 'application/json',
            },
        }).then(response => {
            if (response.status !== 204) {
                throw new Error(`Error deleting account: ${response.statusText}`);
            }

            window.location.href = `http://localhost:8080${prevURL}`;
        }).then(data => {
            console.log(data.message);

        }).catch(error => {
                console.error(error);
        });
    };
}

var dropdownBtn = document.getElementById('dropdownBtn');
var dropdownContent = document.getElementById('myDropdown');

dropdownBtn.addEventListener('click', function() {
    if (dropdownContent.style.display === 'block') {
        dropdownContent.style.display = 'none';
    } else {
        dropdownContent.style.display = 'block';
    }
});

window.onclick = function(event) {
    if (!event.target.matches('#dropdownBtn')) {
        if (dropdownContent.style.display === 'block') {
            dropdownContent.style.display = 'none';
        }
    }
};

saveBtn = document.getElementById('account_Save');

if(saveBtn && accountId === '-1') {
    saveBtn.onclick = function (event) {

        event.preventDefault();

        const formData = {
            name: document.getElementById('account_name').value,
            type: document.getElementById('account_type').value,
            expiration: document.getElementById('account_expiration').value,
            employee: document.getElementById('account_employee').value
        };

        const encodedFormData = new URLSearchParams(formData).toString();

        console.log(formData);

        const apiUrl = `http://localhost:8080/api/employee/${selectedEmployee}`;

        fetch(apiUrl, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: encodedFormData,
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error(`Error submitting form: ${response.statusText}`);
                }

                return response.json();
            })
            .then(data => {
                console.log(data.message);

                redirectToPage();
            })
            .catch(error => {
                console.error(error);
            });
    };
}

function redirectToPage(){
    window.location.href = `http://localhost:8080/employee_accounts_detail/${selectedEmployee}`;
}
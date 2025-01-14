var input = document.getElementById('autocompleteInput');
var optionsContainer = document.getElementById('autocompleteOptions');
var roles = ['Admin', 'User', 'Editor', 'Guest', 'Maintainer', 'Supervisor'];

var selectedValue = localStorage.getItem('selectedRole');
if (selectedValue) {
    input.value = selectedValue;
}

input.addEventListener('input', function() {
    var inputValue = this.value.toLowerCase();
    closeOptions();

    if (!inputValue) {
        return;
    }

    var filteredRoles = roles.filter(function(role) {
        return role.toLowerCase().includes(inputValue);
    });
    // filteredRoles = filteredRoles.slice(0,4);
    displayOptions(filteredRoles);
});

function displayOptions(options) {
    if (options.length === 0) {
        return;
    }

    optionsContainer.innerHTML = '';
    options.forEach(function(option) {
        var optionElement = document.createElement('div');
        optionElement.className = 'autocomplete-option';
        optionElement.textContent = option;

        optionElement.addEventListener('click', function() {
            input.value = option;
            closeOptions();

            localStorage.setItem('selectedRole', option);
        });

        optionsContainer.appendChild(optionElement);
    });

    optionsContainer.style.display = 'block';
}

function closeOptions() {
    optionsContainer.style.display = 'none';
}

const employeeSearchInput = document.getElementById('employeeSearch');

let employeeNames;

fetch('http://localhost:8080/api/employees')
    .then(response => {
        if (!response.ok) {
            throw new Error('Failed to fetch employee data');
        }
        return response.json();
    })
    .then(data => {
        if (!Array.isArray(data)) {
            throw new Error('Invalid response format. Expected an array.');
        }

        employeeNames = extractEmployeeNames(data);
    })
    .catch(error => {
        console.error('Error fetching employee data:', error);
    });

function extractEmployeeNames(employees) {
    return employees.map(employee => employee.first_name + ' ' + employee.last_name);
}


employeeSearchInput.addEventListener('input', function () {
    const inputText = this.value.toLowerCase();
    const matchingNames = employeeNames.filter(name => name.toLowerCase().includes(inputText));

    displayAutocompleteSuggestions(matchingNames);
});

function displayAutocompleteSuggestions(suggestions) {
    var suggestionsContainer = document.getElementById('autocompleteOptionsName');

    suggestionsContainer.innerHTML = '';

    suggestions.forEach(suggestion => {
        var suggestionsElement = document.createElement('div');
        suggestionsElement.className = 'autocomplete-option';
        suggestionsElement.textContent = suggestion;

        suggestionsElement.addEventListener('click', function() {
            employeeSearchInput.value = suggestion;
            suggestionsContainer.style.display = 'none';
            document.getElementById('search_form').submit();
        });

        suggestionsContainer.appendChild(suggestionsElement);
    });

    suggestionsContainer.style.display = 'block';
}
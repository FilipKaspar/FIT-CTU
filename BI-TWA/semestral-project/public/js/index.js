class Modal {
    constructor(modalId, openBtnId) {
        this.modal = document.getElementById(modalId);
        this.openBtn = document.getElementById(openBtnId);
        this.closeBtn = this.modal.querySelector('.closeModal');

        this.openBtn.addEventListener('click', () => this.openModal());
        this.closeBtn.addEventListener('click', () => this.closeModal());
    }



    openModal() {
        this.modal.style.display = 'block';
    }

    closeModal() {
        this.modal.style.display = 'none';
    }

    closeOutsideModal(event) {
        if (event.target === this.modal) {
            this.closeModal();
        }
    }

    handleKeyPress(event) {
        if (event.key === 'Escape') {
            this.closeModal();
        }
    }
}

const myModal = new Modal('myModal', 'openModalBtn');

window.addEventListener('click', (event) => myModal.closeOutsideModal(event));
document.addEventListener('keydown', (event) => myModal.handleKeyPress(event));

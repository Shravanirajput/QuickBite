const menu = [
    { id: 101, name: 'Veg Sandwich', price: 50, stock: 8, icon: '🥪' },
    { id: 102, name: 'Cold Coffee', price: 60, stock: 4, icon: '☕' },
    { id: 103, name: 'French Fries', price: 80, stock: 10, icon: '🍟' },
    { id: 104, name: 'Paneer Wrap', price: 90, stock: 7, icon: '🌯' },
    { id: 105, name: 'Masala Chai', price: 30, stock: 12, icon: '🍵' },
    { id: 106, name: 'Samosa Plate', price: 40, stock: 10, icon: '🥟' },
    { id: 107, name: 'Lemon Soda', price: 45, stock: 8, icon: '🍋' },
    { id: 108, name: 'Chocolate Muffin', price: 55, stock: 4, icon: '🧁' }
];

const orders = [
    { id: 1001, student: 'Shravani', table: 101, items: 'Cold Coffee x 2', total: 126, status: 'PLACED' }
];
const cart = {};
const menuGrid = document.querySelector('#menuGrid');
const cartItems = document.querySelector('#cartItems');
const searchInput = document.querySelector('#menuSearch');

function money(value) { return `Rs. ${value.toFixed(2)}`; }
function showToast(message) {
    const toast = document.querySelector('#toast');
    toast.textContent = message;
    toast.classList.add('show');
    window.clearTimeout(showToast.timer);
    showToast.timer = window.setTimeout(() => toast.classList.remove('show'), 2400);
}
function renderMenu(filter = '') {
    const visible = menu.filter(item => item.name.toLowerCase().includes(filter.toLowerCase()));
    menuGrid.innerHTML = visible.map(item => `
        <article class="menu-card">
            <div class="food-art">${item.icon}</div>
            <div><h3 class="food-name">${item.name}</h3><div class="food-meta"><span class="food-price">${money(item.price)}</span><span class="stock-label">${item.stock} available</span></div></div>
            <button class="add-button" data-add="${item.id}" ${item.stock === 0 ? 'disabled' : ''} aria-label="Add ${item.name}">+</button>
        </article>`).join('') || '<p>No menu items found.</p>';
    menuGrid.querySelectorAll('[data-add]').forEach(button => button.addEventListener('click', () => addToCart(Number(button.dataset.add))));
}
function addToCart(id) {
    const item = menu.find(food => food.id === id);
    if (!item || item.stock <= (cart[id] || 0)) return showToast('That is all we have in stock today.');
    cart[id] = (cart[id] || 0) + 1;
    renderCart();
    showToast(`${item.name} added to your order`);
}
function changeQuantity(id, amount) {
    cart[id] = (cart[id] || 0) + amount;
    if (cart[id] <= 0) delete cart[id];
    renderCart();
}
function renderCart() {
    const entries = Object.entries(cart);
    const itemCount = entries.reduce((sum, [, quantity]) => sum + quantity, 0);
    document.querySelector('#cartCount').textContent = itemCount ? `${itemCount} item${itemCount === 1 ? '' : 's'} in your cart` : 'Cart is empty';
    if (!entries.length) {
        cartItems.innerHTML = '<div class="empty-cart"><span>+</span><p>Add something delicious<br>from the menu</p></div>';
    } else {
        cartItems.innerHTML = entries.map(([id, quantity]) => {
            const item = menu.find(food => food.id === Number(id));
            return `<div class="cart-row"><div><strong>${item.name}</strong><small>${money(item.price)} each</small></div><div class="quantity-controls"><button data-change="${id}" data-amount="-1">−</button><span>${quantity}</span><button data-change="${id}" data-amount="1">+</button></div></div>`;
        }).join('');
        cartItems.querySelectorAll('[data-change]').forEach(button => button.addEventListener('click', () => changeQuantity(Number(button.dataset.change), Number(button.dataset.amount))));
    }
    const subtotal = entries.reduce((sum, [id, quantity]) => sum + menu.find(item => item.id === Number(id)).price * quantity, 0);
    document.querySelector('#subtotal').textContent = money(subtotal);
    document.querySelector('#tax').textContent = money(subtotal * .05);
    document.querySelector('#total').textContent = money(subtotal * 1.05);
    document.querySelector('#checkoutButton').disabled = entries.length === 0;
}
function renderQueue() {
    const stages = { PLACED: 'PREPARING', PREPARING: 'READY', READY: 'COMPLETED' };
    document.querySelector('#queueList').innerHTML = orders.map(order => `
        <article class="queue-item"><div class="queue-top"><strong>Order #${order.id} · ${order.student}</strong><span class="status ${order.status.toLowerCase()}">${order.status}</span></div><div class="queue-details"><span>Table ${order.table} · ${order.items}</span><strong>${money(order.total)}</strong></div>${stages[order.status] ? `<button class="queue-action" data-next="${order.id}">Mark ${stages[order.status].toLowerCase()}</button>` : ''}</article>`).join('');
    document.querySelectorAll('[data-next]').forEach(button => button.addEventListener('click', () => {
        const order = orders.find(item => item.id === Number(button.dataset.next));
        order.status = stages[order.status];
        renderQueue(); renderStats(); showToast(`Order #${order.id} marked ${order.status.toLowerCase()}`);
    }));
}
function renderStats() {
    const active = orders.filter(order => !['COMPLETED', 'CANCELLED'].includes(order.status));
    document.querySelector('#activeOrders').textContent = active.length;
    document.querySelector('#readyOrders').textContent = orders.filter(order => order.status === 'READY').length;
    document.querySelector('#salesTotal').textContent = money(orders.filter(order => order.status === 'COMPLETED').reduce((sum, order) => sum + order.total, 0));
}
function renderStock() {
    document.querySelector('#stockList').innerHTML = menu.map(item => `<div class="stock-row"><div><strong>${item.name}</strong><small>${item.stock} units left</small><div class="stock-bar"><span style="width:${Math.min(item.stock * 10, 100)}%"></span></div></div><strong class="stock-value">${item.stock}</strong></div>`).join('');
}

document.querySelectorAll('.nav-link').forEach(button => button.addEventListener('click', () => {
    document.querySelectorAll('.nav-link').forEach(item => item.classList.remove('active'));
    document.querySelectorAll('.view').forEach(view => view.classList.remove('active-view'));
    button.classList.add('active');
    document.querySelector(`#${button.dataset.view}`).classList.add('active-view');
    document.querySelector('#pageTitle').textContent = button.dataset.view === 'staffView' ? 'Keep the line moving.' : 'Your best break starts here.';
}));
searchInput.addEventListener('input', event => renderMenu(event.target.value));
document.querySelector('#checkoutButton').addEventListener('click', () => { showToast('Order preview ready. Your kitchen ticket is all set.'); });
document.querySelector('#refreshQueue').addEventListener('click', () => { renderQueue(); showToast('Queue refreshed'); });
renderMenu(); renderCart(); renderQueue(); renderStats(); renderStock();

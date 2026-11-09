/**
 * ============================================================================
 * SMART RENTAL MANAGEMENT SYSTEM - API CLIENT & UTILITY MODULE
 * Communicates with C++ REST API endpoints using standard JSON fetch.
 * ============================================================================
 */

const API_BASE = (typeof window !== 'undefined' && window.location && window.location.origin && window.location.origin !== 'null' && window.location.protocol.startsWith('http'))
  ? `${window.location.origin}/api`
  : '/api';

const API = {
  getToken() {
    return localStorage.getItem('srms_token') || '';
  },

  setToken(token) {
    localStorage.setItem('srms_token', token);
  },

  removeToken() {
    localStorage.removeItem('srms_token');
    localStorage.removeItem('srms_user');
  },

  getUser() {
    try {
      const u = localStorage.getItem('srms_user');
      return u ? JSON.parse(u) : null;
    } catch {
      return null;
    }
  },

  setUser(user) {
    localStorage.setItem('srms_user', JSON.stringify(user));
  },

  async request(endpoint, options = {}) {
    const cleanEndpoint = endpoint.startsWith('/') ? endpoint : '/' + endpoint;
    const url = `${API_BASE}${cleanEndpoint}`;
    const headers = {
      'Content-Type': 'application/json',
      ...(options.headers || {})
    };

    const token = this.getToken();
    if (token) {
      headers['Authorization'] = `Bearer ${token}`;
    }

    try {
      const res = await fetch(url, { ...options, headers });
      const contentType = res.headers.get('content-type') || '';
      let data = null;
      if (contentType.includes('application/json')) {
        data = await res.json();
      } else {
        data = await res.text();
      }

      if (!res.ok) {
        const errorMsg = (data && (data.message || data.error)) ? (data.message || data.error) : `HTTP Error ${res.status}`;
        throw new Error(errorMsg);
      }
      return data;
    } catch (err) {
      console.warn(`API [${options.method || 'GET'} ${cleanEndpoint}] notice:`, err.message);
      throw err;
    }
  },

  async call(endpoint, method = 'GET', body = null) {
    const opts = { method };
    if (body) {
      opts.body = JSON.stringify(body);
    }
    return this.request(endpoint, opts);
  },

  get(endpoint, params = {}) {
    const query = new URLSearchParams(params).toString();
    const url = query ? `${endpoint}?${query}` : endpoint;
    return this.request(url, { method: 'GET' });
  },

  post(endpoint, body = {}) {
    return this.request(endpoint, {
      method: 'POST',
      body: JSON.stringify(body)
    });
  },

  put(endpoint, body = {}) {
    return this.request(endpoint, {
      method: 'PUT',
      body: JSON.stringify(body)
    });
  },

  delete(endpoint) {
    return this.request(endpoint, { method: 'DELETE' });
  }
};

// Auth alias object
const Auth = {
  getUser: () => API.getUser(),
  setUser: (u) => API.setUser(u),
  getToken: () => API.getToken(),
  setToken: (t) => API.setToken(t),
  clearSession: () => API.removeToken()
};

// ----------------------------------------------------------------------------
// Formatting & Security Sanitization Helpers
// ----------------------------------------------------------------------------
function escapeHtml(str) {
  if (str === null || str === undefined) return '';
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');
}
window.escapeHtml = escapeHtml;

function formatCurrency(amount) {
  const num = Number(amount) || 0;
  return '₹' + num.toLocaleString('en-IN', {
    maximumFractionDigits: 2,
    minimumFractionDigits: 0
  });
}

function formatDate(dateStr) {
  if (!dateStr) return '-';
  const d = new Date(dateStr);
  if (isNaN(d.getTime())) return dateStr;
  return d.toLocaleDateString('en-IN', {
    day: 'numeric',
    month: 'short',
    year: 'numeric'
  });
}

// ----------------------------------------------------------------------------
// Toast Notification Engine
// ----------------------------------------------------------------------------
function showToast(message, type = 'success') {
  let container = document.getElementById('toastContainer');
  if (!container) {
    container = document.createElement('div');
    container.id = 'toastContainer';
    container.className = 'toast-container position-fixed top-0 end-0 p-3';
    container.style.zIndex = '99999';
    document.body.appendChild(container);
  }

  const toastId = 'toast_' + Date.now();
  const bgClass = type === 'success' ? 'bg-success' : (type === 'error' ? 'bg-danger' : 'bg-primary');
  const icon = type === 'success' ? 'fa-check-circle' : (type === 'error' ? 'fa-exclamation-triangle' : 'fa-info-circle');

  const html = `
    <div id="${toastId}" class="toast align-items-center text-white ${bgClass} border-0 show shadow-lg mb-2" role="alert" aria-live="assertive" aria-atomic="true">
      <div class="d-flex">
        <div class="toast-body d-flex align-items-center gap-2">
          <i class="fa-solid ${icon}"></i>
          <span>${message}</span>
        </div>
        <button type="button" class="btn-close btn-close-white me-2 m-auto" data-bs-dismiss="toast" aria-label="Close"></button>
      </div>
    </div>
  `;

  container.insertAdjacentHTML('beforeend', html);
  setTimeout(() => {
    const el = document.getElementById(toastId);
    if (el) el.remove();
  }, 4500);
}

const Toast = {
  success: (msg) => showToast(msg, 'success'),
  error: (msg) => showToast(msg, 'error'),
  info: (msg) => showToast(msg, 'info')
};

// ----------------------------------------------------------------------------
// Shared Navbar & Auth State Controller
// ----------------------------------------------------------------------------
function updateNavbar() {
  const user = API.getUser();
  const authLinks = document.getElementById('navbarAuthLinks');
  if (!authLinks) return;

  if (user && API.getToken()) {
    const isAdm = user.role === 'admin';
    authLinks.innerHTML = `
      <li class="nav-item dropdown">
        <a class="nav-link dropdown-toggle d-flex align-items-center gap-2" href="#" role="button" data-bs-toggle="dropdown">
          <i class="fa-solid ${isAdm ? 'fa-user-shield' : 'fa-user-circle'} text-info"></i>
          <span>${escapeHtml(user.full_name || user.name || user.email)}</span>
        </a>
        <ul class="dropdown-menu dropdown-menu-end shadow border-0">
          <li><span class="dropdown-item-text text-muted small">${isAdm ? 'Administrator' : 'Customer Account'}</span></li>
          <li><hr class="dropdown-divider"></li>
          <li>
            <a class="dropdown-item" href="${isAdm ? 'admin-dashboard.html' : 'customer-dashboard.html'}">
              <i class="fa-solid fa-gauge me-2 text-primary"></i>Dashboard
            </a>
          </li>
          ${!isAdm ? `
          <li>
            <a class="dropdown-item" href="my-bookings.html">
              <i class="fa-solid fa-calendar-check me-2 text-success"></i>My Bookings
            </a>
          </li>
          <li>
            <a class="dropdown-item" href="rental-history.html">
              <i class="fa-solid fa-clock-rotate-left me-2 text-warning"></i>Rental History
            </a>
          </li>
          ` : `
          <li>
            <a class="dropdown-item" href="manage-items.html">
              <i class="fa-solid fa-boxes-stacked me-2 text-primary"></i>Manage Inventory
            </a>
          </li>
          <li>
            <a class="dropdown-item" href="manage-bookings.html">
              <i class="fa-solid fa-list-check me-2 text-success"></i>Manage Bookings
            </a>
          </li>
          <li>
            <a class="dropdown-item" href="reports.html">
              <i class="fa-solid fa-chart-line me-2 text-info"></i>Analytics & Reports
            </a>
          </li>
          `}
          <li>
            <a class="dropdown-item" href="${isAdm ? 'admin-profile.html' : 'profile.html'}">
              <i class="fa-solid fa-gear me-2 text-secondary"></i>Profile
            </a>
          </li>
          <li><hr class="dropdown-divider"></li>
          <li>
            <a class="dropdown-item text-danger" href="javascript:void(0)" onclick="handleLogout()">
              <i class="fa-solid fa-right-from-bracket me-2"></i>Logout
            </a>
          </li>
        </ul>
      </li>
    `;
  } else {
    authLinks.innerHTML = `
      <li class="nav-item">
        <a class="nav-link" href="login.html"><i class="fa-solid fa-right-to-bracket me-1"></i>Login</a>
      </li>
      <li class="nav-item ms-lg-2">
        <a class="nav-link nav-pill-btn" href="register.html"><i class="fa-solid fa-user-plus me-1"></i>Register</a>
      </li>
    `;
  }
}

async function handleLogout() {
  try {
    await API.post('/auth/logout');
  } catch {}
  API.removeToken();
  showToast('Logged out successfully', 'info');
  setTimeout(() => {
    window.location.href = 'login.html';
  }, 600);
}

document.addEventListener('DOMContentLoaded', updateNavbar);

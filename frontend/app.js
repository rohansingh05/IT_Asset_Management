// SAMPLE DATA
// This will be replaced by live data from the C++ backend later.
const API_BASE_URL = "http://127.0.0.1:8080";

const assets = [
  {
    id: 1001,
    type: "Laptop",
    brand: "HP",
    model: "EliteBook",
    department: "IT",
    employee: "Rahul Sharma",
    status: "In Use",
  },
  {
    id: 1002,
    type: "Desktop",
    brand: "Dell",
    model: "OptiPlex",
    department: "Finance",
    employee: "None",
    status: "In Stock",
  },
  {
    id: 1003,
    type: "Printer",
    brand: "Canon",
    model: "LBP 2900",
    department: "Administration",
    employee: "None",
    status: "Under Repair",
  },
  {
    id: 1004,
    type: "Laptop",
    brand: "Lenovo",
    model: "ThinkPad",
    department: "HR",
    employee: "Priya Singh",
    status: "In Use",
  },
  {
    id: 1005,
    type: "Projector",
    brand: "Epson",
    model: "EB-X06",
    department: "Training",
    employee: "None",
    status: "Retired",
  },
];

let maintenanceRecords = [];
let warrantyRecords = [];

// HELPER: Count assets by status
function countByStatus(status) {
  return assets.filter((asset) => asset.status === status).length;
}

// HELPER: Safely display text received as data
function escapeHTML(value) {
  return String(value).replace(/[&<>"']/g, (character) => {
    const entities = {
      "&": "&amp;",
      "<": "&lt;",
      ">": "&gt;",
      '"': "&quot;",
      "'": "&#39;",
    };

    return entities[character];
  });
}

// 1. UPDATE SUMMARY CARDS
function updateSummary() {
  document.getElementById("totalAssets").textContent = assets.length;

  document.getElementById("inUseAssets").textContent = countByStatus("In Use");

  document.getElementById("inStockAssets").textContent =
    countByStatus("In Stock");

  document.getElementById("repairAssets").textContent =
    countByStatus("Under Repair");

  document.getElementById("retiredAssets").textContent =
    countByStatus("Retired");
}

// 2. UPDATE STATUS BREAKDOWN
function updateStatusBreakdown() {
  const container = document.getElementById("statusBreakdown");

  const statuses = [
    { name: "In Use", color: "#16a34a" },
    { name: "In Stock", color: "#7c3aed" },
    { name: "Under Repair", color: "#d97706" },
    { name: "Retired", color: "#dc2626" },
  ];

  container.innerHTML = statuses
    .map((item) => {
      const count = countByStatus(item.name);

      const percentage =
        assets.length === 0 ? 0 : (count / assets.length) * 100;

      return `
            <div class="status-item">
                <span class="status-name">${item.name}</span>

                <div class="status-track">
                    <div
                        class="status-fill"
                        style="width: ${percentage}%; background: ${item.color};"
                    ></div>
                </div>

                <span class="status-value">${count}</span>
            </div>
        `;
    })
    .join("");
}

// 3. DISPLAY ASSET TABLE
function displayAssets(searchText = "") {
  const tableBody = document.getElementById("assetTableBody");

  const search = searchText.trim().toLowerCase();

  const filteredAssets = assets.filter((asset) => {
    const searchableText = [
      asset.id,
      asset.type,
      asset.brand,
      asset.model,
      asset.department,
      asset.employee,
      asset.status,
    ]
      .join(" ")
      .toLowerCase();

    return searchableText.includes(search);
  });

  if (filteredAssets.length === 0) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="5">No matching assets found.</td>
            </tr>
        `;
  } else {
    tableBody.innerHTML = filteredAssets
      .map((asset) => {
        const statusClasses = {
          "In Use": "badge-in-use",
          "In Stock": "badge-in-stock",
          "Under Repair": "badge-under-repair",
          Retired: "badge-retired",
        };

        const statusClass = statusClasses[asset.status] || "";

        return `
                <tr>
                    <td class="asset-id">
                        ${escapeHTML(asset.id)}
                    </td>

                    <td>
                        <div class="asset-name">
                            ${escapeHTML(asset.brand)}
                        </div>
                        <div class="asset-subtitle">
                            ${escapeHTML(asset.type)} ·
                            ${escapeHTML(asset.model)}
                        </div>
                    </td>

                    <td>${escapeHTML(asset.department)}</td>

                    <td>${escapeHTML(asset.employee)}</td>

                    <td>
                        <span class="status-badge ${statusClass}">
                            ${escapeHTML(asset.status)}
                        </span>
                    </td>
                </tr>
            `;
      })
      .join("");
  }

  document.getElementById("assetCount").textContent =
    `Showing ${filteredAssets.length} of ${assets.length} assets`;
}

// ------------------------------------
// ASSET MANAGEMENT PAGE
// ------------------------------------

// Update asset management summary cards
function updateManagementSummary() {
  document.getElementById("managementTotal").textContent = assets.length;

  document.getElementById("managementInUse").textContent =
    countByStatus("In Use");

  document.getElementById("managementInStock").textContent =
    countByStatus("In Stock");
}

// Format purchase cost as Indian Rupees
function formatCurrency(amount) {
  const value = Number(amount);

  if (!Number.isFinite(value)) {
    return "—";
  }

  return new Intl.NumberFormat("en-IN", {
    style: "currency",
    currency: "INR",
    maximumFractionDigits: 2,
  }).format(value);
}

// Display all assets with filters
function displayAllAssets() {
  const tableBody = document.getElementById("allAssetsTableBody");

  const search = document
    .getElementById("managementSearch")
    .value.trim()
    .toLowerCase();

  const statusFilter = document.getElementById("managementStatusFilter").value;

  const typeFilter = document.getElementById("managementTypeFilter").value;

  // Search and filter records
  const filteredAssets = assets.filter((asset) => {
    const searchableText = [
      asset.id,
      asset.type,
      asset.brand,
      asset.model,
      asset.serialNumber,
      asset.department,
      asset.location,
      asset.employee,
      asset.status,
    ]
      .join(" ")
      .toLowerCase();

    const matchesSearch = searchableText.includes(search);

    const matchesStatus = statusFilter === "" || asset.status === statusFilter;

    const matchesType = typeFilter === "" || asset.type === typeFilter;

    return matchesSearch && matchesStatus && matchesType;
  });

  // Show empty result message
  if (filteredAssets.length === 0) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="8">
                    No matching assets found.
                </td>
            </tr>
        `;
  } else {
    const statusClasses = {
      "In Use": "badge-in-use",
      "In Stock": "badge-in-stock",
      "Under Repair": "badge-under-repair",
      Retired: "badge-retired",
    };

    tableBody.innerHTML = filteredAssets
      .map((asset) => {
        const statusClass = statusClasses[asset.status] || "";

        return `
                <tr>

                    <td class="asset-id">
                        ${escapeHTML(asset.id)}
                    </td>

                    <td>
                        <div class="asset-name">
                            ${escapeHTML(asset.brand || "—")}
                        </div>

                        <div class="asset-subtitle">
                            ${escapeHTML(asset.type || "—")}
                            ·
                            ${escapeHTML(asset.model || "—")}
                        </div>
                    </td>

                    <td>
                        ${escapeHTML(asset.serialNumber || "—")}
                    </td>

                    <td>
                        ${escapeHTML(asset.department || "—")}
                    </td>

                    <td>
                        ${escapeHTML(asset.location || "—")}
                    </td>

                    <td>
                        ${escapeHTML(asset.employee || "None")}
                    </td>

                    <td>
                        ${escapeHTML(formatCurrency(asset.purchaseCost))}
                    </td>

                    <td>
                        <span class="status-badge ${statusClass}">
                            ${escapeHTML(asset.status)}
                        </span>
                    </td>

                </tr>
            `;
      })
      .join("");
  }

  // Update record count
  document.getElementById("allAssetsCount").textContent =
    `Showing ${filteredAssets.length} of ${assets.length} assets`;
}

// Search event
document
  .getElementById("managementSearch")
  .addEventListener("input", displayAllAssets);

// Status filter event
document
  .getElementById("managementStatusFilter")
  .addEventListener("change", displayAllAssets);

// Asset type filter event
document
  .getElementById("managementTypeFilter")
  .addEventListener("change", displayAllAssets);

// ------------------------------------
// PAGE NAVIGATION
// ------------------------------------

const dashboardElements = [
  document.getElementById("dashboardHeader"),
  document.getElementById("dashboardStats"),
  document.getElementById("dashboardOverview"),
  document.getElementById("dashboardRecentAssets"),
];

const reportsPage = document.getElementById("reportsPage");

const reportsNav = document.getElementById("reportsNav");

reportsNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("reports");
});

const assetManagementPage = document.getElementById("assetManagementPage");

const allocationPage = document.getElementById("allocationPage");

const maintenancePage = document.getElementById("maintenancePage");

const warrantyPage = document.getElementById("warrantyPage");

const dashboardNav = document.getElementById("dashboardNav");

const assetsNav = document.getElementById("assetsNav");

const allocationNav = document.getElementById("allocationNav");

const maintenanceNav = document.getElementById("maintenanceNav");

const warrantyNav = document.getElementById("warrantyNav");

async function showPage(page) {
  const showDashboard = page === "dashboard";
  const showAssets = page === "assets";
  const showAllocation = page === "allocation";
  const showMaintenance = page === "maintenance";
  const showWarranty = page === "warranty";

  const showReports = page === "reports";

  reportsPage.hidden = !showReports;

  reportsNav.classList.toggle("active", showReports);

  if (showReports) {
    await refreshReportsData();
  }

  dashboardElements.forEach((element) => {
    element.hidden = !showDashboard;
  });

  assetManagementPage.hidden = !showAssets;
  allocationPage.hidden = !showAllocation;
  maintenancePage.hidden = !showMaintenance;
  warrantyPage.hidden = !showWarranty;

  dashboardNav.classList.toggle("active", showDashboard);
  assetsNav.classList.toggle("active", showAssets);
  allocationNav.classList.toggle("active", showAllocation);
  maintenanceNav.classList.toggle("active", showMaintenance);
  warrantyNav.classList.toggle("active", showWarranty);

  if (showAssets) {
    updateManagementSummary();
    displayAllAssets();
  }

  if (showAllocation) {
    await refreshAllocationData();
  }

  if (showMaintenance) {
    await refreshMaintenanceData();
  }

  if (showWarranty) {
    await refreshWarrantyData();
  }
}

// Dashboard
dashboardNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("dashboard");
});

// Asset Management
assetsNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("assets");
});

// Allocation and Transfer
allocationNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("allocation");
});

// Maintenance
maintenanceNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("maintenance");
});

// Warranty
warrantyNav.addEventListener("click", (event) => {
  event.preventDefault();
  showPage("warranty");
});
// Load assets from the C++ backend
async function loadAssetsFromBackend() {
  const demoLabel = document.querySelector(".demo-label");

  try {
    const response = await fetch(`${API_BASE_URL}/api/assets`);

    if (!response.ok) {
      throw new Error("Could not load assets from backend.");
    }

    const backendAssets = await response.json();

    if (!Array.isArray(backendAssets)) {
      throw new Error("Invalid asset data received.");
    }

    // Replace sample data with actual backend records
    assets.splice(
      0,
      assets.length,
      ...backendAssets.map((asset) => ({
        id: asset.assetID,
        type: asset.assetType,
        brand: asset.brand,
        model: asset.model,
        serialNumber: asset.serialNumber,
        purchaseDate: asset.purchaseDate,
        purchaseCost: asset.purchaseCost,
        warrantyEndDate: asset.warrantyEndDate,
        location: asset.location,
        department: asset.department,
        employee: asset.assignedEmployee,
        status: asset.status,
      })),
    );

    return true;
  } catch (error) {
    console.warn("Backend connection unavailable:", error);

    demoLabel.textContent = "Backend offline — sample data";

    return false;
  }
}

// 4. SEARCH FUNCTIONALITY
document.getElementById("assetSearch").addEventListener("input", (event) => {
  displayAssets(event.target.value);
});

// 5. OPEN AND CLOSE REGISTRATION FORM

const assetDialog = document.getElementById("assetDialog");
const assetForm = document.getElementById("assetForm");
const formMessage = document.getElementById("formMessage");

document.getElementById("addAssetBtn").addEventListener("click", () => {
  formMessage.textContent = "";
  assetDialog.showModal();
});

document.getElementById("addAssetFromListBtn").addEventListener("click", () => {
  formMessage.textContent = "";
  assetDialog.showModal();
});

document.getElementById("closeAssetDialog").addEventListener("click", () => {
  assetDialog.close();
});

document.getElementById("cancelAssetBtn").addEventListener("click", () => {
  assetDialog.close();
});

// Convert HTML date (YYYY-MM-DD) to the backend's DD-MM-YYYY format
function formatDateForBackend(dateValue) {
  const [year, month, day] = dateValue.split("-");
  return `${day}-${month}-${year}`;
}

// 6. VALIDATE REGISTRATION FORM

// Submit asset registration to C++ backend
assetForm.addEventListener("submit", async (event) => {
  event.preventDefault();

  formMessage.style.color = "#b45309";
  formMessage.textContent = "Validating asset details...";

  const purchaseDate = assetForm.elements.purchaseDate.value;
  const warrantyEndDate = assetForm.elements.warrantyEndDate.value;

  if (warrantyEndDate < purchaseDate) {
    formMessage.textContent =
      "Warranty end date cannot be earlier than purchase date.";

    return;
  }

  const assetData = {
    assetType: assetForm.elements.assetType.value.trim(),

    brand: assetForm.elements.brand.value.trim(),

    model: assetForm.elements.model.value.trim(),

    serialNumber: assetForm.elements.serialNumber.value.trim(),

    purchaseDate: formatDateForBackend(purchaseDate),

    purchaseCost: Number(assetForm.elements.purchaseCost.value),

    warrantyEndDate: formatDateForBackend(warrantyEndDate),

    location: assetForm.elements.location.value.trim(),

    department: assetForm.elements.department.value.trim(),

    assignedEmployee:
      assetForm.elements.assignedEmployee.value.trim() || "None",
  };

  try {

    const response = await fetch(`${API_BASE_URL}/api/assets`, {
      method: "POST",

      headers: {
        "Content-Type": "application/json",
      },

      // Notice: Asset ID is NOT sent.
      body: JSON.stringify(assetData),
    });

    const result = await response.json();

    if (!response.ok) {
      throw new Error(result.message || "Asset registration failed.");
    }

    // Reload the dashboard from the C++ backend
    await loadAssetsFromBackend();

    updateSummary();
    updateStatusBreakdown();
    displayAssets(document.getElementById("assetSearch").value);
    updateManagementSummary();
    displayAllAssets();

    // Close and reset form
    assetForm.reset();
    assetDialog.close();

    alert(
      "Asset registered successfully!\n\n" +
        "Generated Asset ID: " +
        result.assetID,
    );
  } catch (error) {
    formMessage.style.color = "#dc2626";

    formMessage.textContent =
      error.message || "Could not connect to C++ backend.";

    console.error("Registration error:", error);
  }
});

// 6. INITIALIZE DASHBOARD
async function initializeDashboard() {
  await loadAssetsFromBackend();

  updateSummary();
  updateStatusBreakdown();
  displayAssets();

  updateManagementSummary();
  displayAllAssets();

  // Load warranty count for the main dashboard
  await refreshWarrantyData();
}

initializeDashboard();

// ------------------------------------
// ASSET SELECTION
// ------------------------------------

function renderAllocationOptions() {
  const allocateSelect = document.getElementById("allocateAssetId");

  const transferSelect = document.getElementById("transferAssetId");

  // Only In Stock assets can be allocated
  const availableAssets = assets.filter((asset) => asset.status === "In Stock");

  // Only In Use assets can be transferred
  const assignedAssets = assets.filter((asset) => asset.status === "In Use");

  allocateSelect.innerHTML = '<option value="">Select available asset</option>';

  transferSelect.innerHTML = '<option value="">Select assigned asset</option>';

  availableAssets.forEach((asset) => {
    const option = document.createElement("option");

    option.value = asset.id;

    option.textContent = `${asset.id} - ${asset.brand} ${asset.model}`;

    allocateSelect.appendChild(option);
  });

  assignedAssets.forEach((asset) => {
    const option = document.createElement("option");

    option.value = asset.id;

    option.textContent = `${asset.id} - ${asset.brand} ${asset.model}`;

    transferSelect.appendChild(option);
  });

  if (availableAssets.length === 0) {
    allocateSelect.innerHTML = '<option value="">No available assets</option>';
  }

  if (assignedAssets.length === 0) {
    transferSelect.innerHTML = '<option value="">No assigned assets</option>';
  }

  updateCurrentEmployee();
}

// Show the current employee when an asset is selected
function updateCurrentEmployee() {
  const selectedID = Number(document.getElementById("transferAssetId").value);

  const selectedAsset = assets.find((asset) => asset.id === selectedID);

  document.getElementById("currentEmployee").textContent = selectedAsset
    ? selectedAsset.employee
    : "Select an asset";
}

document
  .getElementById("transferAssetId")
  .addEventListener("change", updateCurrentEmployee);

// ------------------------------------
// LIFECYCLE HISTORY
// ------------------------------------

async function loadLifecycleHistory() {
  const tableBody = document.getElementById("lifecycleTableBody");

  try {
    const response = await fetch(`${API_BASE_URL}/api/lifecycle`);

    if (!response.ok) {
      throw new Error("Unable to load lifecycle history.");
    }

    const history = await response.json();

    if (!Array.isArray(history)) {
      throw new Error("Invalid lifecycle data.");
    }

    // Display most recent activity first
    const recentHistory = [...history].reverse().slice(0, 20);

    if (recentHistory.length === 0) {
      tableBody.innerHTML = `
                <tr>
                    <td colspan="5">
                        No lifecycle activities recorded yet.
                    </td>
                </tr>
            `;
    } else {
      tableBody.innerHTML = recentHistory
        .map((record) => {
          return `
                    <tr>
                        <td class="asset-id">
                            ${escapeHTML(record.assetID)}
                        </td>

                        <td>
                            ${escapeHTML(record.action)}
                        </td>

                        <td>
                            ${escapeHTML(record.date)}
                        </td>

                        <td>
                            ${escapeHTML(record.employee)}
                        </td>

                        <td>
                            ${escapeHTML(record.status)}
                        </td>
                    </tr>
                `;
        })
        .join("");
    }

    document.getElementById("lifecycleCount").textContent =
      `Showing ${recentHistory.length} recent activities`;
  } catch (error) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="5">
                    ${escapeHTML(error.message)}
                </td>
            </tr>
        `;

    console.error("Lifecycle history error:", error);
  }
}

// ------------------------------------
// LOAD MAINTENANCE RECORDS
// ------------------------------------

async function loadMaintenanceRecords() {
  const response = await fetch(`${API_BASE_URL}/api/maintenance`);

  if (!response.ok) {
    throw new Error("Could not load maintenance records.");
  }

  const records = await response.json();

  if (!Array.isArray(records)) {
    throw new Error("Invalid maintenance data received.");
  }

  maintenanceRecords.splice(0, maintenanceRecords.length, ...records);

  return true;
}

// Refresh all allocation data
async function refreshAllocationData() {
  await loadAssetsFromBackend();

  renderAllocationOptions();

  await loadLifecycleHistory();

  updateSummary();
  updateStatusBreakdown();
  displayAssets();

  updateManagementSummary();
  displayAllAssets();
}

// Refresh history button
document
  .getElementById("refreshHistoryBtn")
  .addEventListener("click", loadLifecycleHistory);

// ------------------------------------
// ALLOCATE ASSET
// ------------------------------------

document
  .getElementById("allocateForm")
  .addEventListener("submit", async (event) => {
    event.preventDefault();

    const message = document.getElementById("allocateMessage");

    message.style.color = "#b45309";
    message.textContent = "Processing allocation...";

    const assetID = Number(document.getElementById("allocateAssetId").value);

    const employee = document.getElementById("allocateEmployee").value.trim();

    const date = document.getElementById("allocateDate").value;

    const allocationData = {
      assetID: assetID,
      employee: employee,
      date: formatDateForBackend(date),
    };

    try {
      const response = await fetch(`${API_BASE_URL}/api/assets/allocate`, {
        method: "POST",

        headers: {
          "Content-Type": "application/json",
        },

        body: JSON.stringify(allocationData),
      });

      const result = await response.json();

      if (!response.ok) {
        throw new Error(result.message || "Allocation failed.");
      }

      // Refresh backend records
      await refreshAllocationData();

      document.getElementById("allocateForm").reset();

      message.style.color = "#15803d";

      message.textContent = `Asset ${result.assetID} allocated successfully to ${result.employee}.`;

      alert(
        "Asset allocated successfully!\n\n" +
          "Asset ID: " +
          result.assetID +
          "\n" +
          "Employee: " +
          result.employee,
      );
    } catch (error) {
      message.style.color = "#dc2626";
      message.textContent = error.message;

      console.error("Allocation error:", error);
    }
  });

// ------------------------------------
// TRANSFER ASSET
// ------------------------------------

document
  .getElementById("transferForm")
  .addEventListener("submit", async (event) => {
    event.preventDefault();

    const message = document.getElementById("transferMessage");

    message.style.color = "#b45309";
    message.textContent = "Processing transfer...";

    const assetID = Number(document.getElementById("transferAssetId").value);

    const newEmployee = document.getElementById("newEmployee").value.trim();

    const date = document.getElementById("transferDate").value;

    const transferData = {
      assetID: assetID,
      newEmployee: newEmployee,
      date: formatDateForBackend(date),
    };

    try {
      const response = await fetch(`${API_BASE_URL}/api/assets/transfer`, {
        method: "POST",

        headers: {
          "Content-Type": "application/json",
        },

        body: JSON.stringify(transferData),
      });

      const result = await response.json();

      if (!response.ok) {
        throw new Error(result.message || "Transfer failed.");
      }

      await refreshAllocationData();

      document.getElementById("transferForm").reset();

      message.style.color = "#15803d";

      message.textContent = `Asset ${result.assetID} transferred successfully to ${result.newEmployee}.`;

      alert(
        "Asset transferred successfully!\n\n" +
          "Asset ID: " +
          result.assetID +
          "\n" +
          "New Employee: " +
          result.newEmployee,
      );
    } catch (error) {
      message.style.color = "#dc2626";
      message.textContent = error.message;

      console.error("Transfer error:", error);
    }
  });

// Set today's date in allocation and transfer forms
function setDefaultOperationDates() {
  const now = new Date();

  const localDate = [
    now.getFullYear(),
    String(now.getMonth() + 1).padStart(2, "0"),
    String(now.getDate()).padStart(2, "0"),
  ].join("-");

  document.getElementById("allocateDate").value = localDate;
  document.getElementById("transferDate").value = localDate;
}

setDefaultOperationDates();

// ------------------------------------
// MAINTENANCE ASSET SELECTION
// ------------------------------------

function renderMaintenanceAssetOptions() {
  const select = document.getElementById("maintenanceAssetId");

  const eligibleAssets = assets.filter(
    (asset) => asset.status === "In Use" || asset.status === "Under Repair",
  );

  select.innerHTML = '<option value="">Select an asset</option>';

  eligibleAssets.forEach((asset) => {
    const option = document.createElement("option");

    option.value = asset.id;

    option.textContent = `${asset.id} - ${asset.brand} ${asset.model} (${asset.status})`;

    select.appendChild(option);
  });

  if (eligibleAssets.length === 0) {
    select.innerHTML = '<option value="">No eligible assets available</option>';
  }
}

// ------------------------------------
// MAINTENANCE DASHBOARD SUMMARY
// ------------------------------------

function updateMaintenanceSummary() {
  const pendingRecords = maintenanceRecords.filter(
    (record) => record.repairStatus === "Pending",
  );

  const completedRecords = maintenanceRecords.filter(
    (record) => record.repairStatus === "Completed",
  );

  const totalCost = maintenanceRecords.reduce(
    (total, record) => total + Number(record.repairCost || 0),
    0,
  );

  document.getElementById("maintenanceTotal").textContent =
    maintenanceRecords.length;

  document.getElementById("maintenancePending").textContent =
    pendingRecords.length;

  document.getElementById("maintenanceCompleted").textContent =
    completedRecords.length;

  document.getElementById("maintenanceExpenditure").textContent =
    formatCurrency(totalCost);
}

// ------------------------------------
// DISPLAY MAINTENANCE HISTORY
// ------------------------------------

function displayMaintenanceRecords() {
  const tableBody = document.getElementById("maintenanceTableBody");

  const search = document
    .getElementById("maintenanceSearch")
    .value.trim()
    .toLowerCase();

  const statusFilter = document.getElementById("maintenanceStatusFilter").value;

  const filteredRecords = maintenanceRecords.filter((record) => {
    const searchableText = [
      record.assetID,
      record.problem,
      record.repairDate,
      record.technician,
      record.repairStatus,
    ]
      .join(" ")
      .toLowerCase();

    return (
      searchableText.includes(search) &&
      (statusFilter === "" || record.repairStatus === statusFilter)
    );
  });

  // Newest records first, based on saved record order
  const recentRecords = [...filteredRecords].reverse();

  if (recentRecords.length === 0) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="6">
                    No matching maintenance records found.
                </td>
            </tr>
        `;
  } else {
    tableBody.innerHTML = recentRecords
      .map((record) => {
        const statusClass =
          record.repairStatus === "Completed"
            ? "badge-in-use"
            : "badge-under-repair";

        return `
                <tr>

                    <td class="asset-id">
                        ${escapeHTML(record.assetID)}
                    </td>

                    <td>
                        ${escapeHTML(record.problem)}
                    </td>

                    <td>
                        ${escapeHTML(record.repairDate)}
                    </td>

                    <td>
                        ${escapeHTML(record.technician)}
                    </td>

                    <td>
                        ${escapeHTML(formatCurrency(record.repairCost))}
                    </td>

                    <td>
                        <span class="status-badge ${statusClass}">
                            ${escapeHTML(record.repairStatus)}
                        </span>
                    </td>

                </tr>
            `;
      })
      .join("");
  }

  document.getElementById("maintenanceRecordCount").textContent =
    `Showing ${filteredRecords.length} of ${maintenanceRecords.length} records`;
}

// ------------------------------------
// REFRESH MAINTENANCE PAGE
// ------------------------------------

async function refreshMaintenanceData() {
  const tableBody = document.getElementById("maintenanceTableBody");

  try {
    await loadAssetsFromBackend();

    await loadMaintenanceRecords();

    renderMaintenanceAssetOptions();

    updateMaintenanceSummary();

    displayMaintenanceRecords();

    // Refresh the other dashboards too
    updateSummary();
    updateStatusBreakdown();
    displayAssets();

    updateManagementSummary();
    displayAllAssets();
  } catch (error) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="6">
                    ${escapeHTML(error.message)}
                </td>
            </tr>
        `;

    console.error("Maintenance refresh error:", error);
  }
}

// Search maintenance records
document
  .getElementById("maintenanceSearch")
  .addEventListener("input", displayMaintenanceRecords);

// Filter maintenance records by status
document
  .getElementById("maintenanceStatusFilter")
  .addEventListener("change", displayMaintenanceRecords);

// Manually refresh maintenance records
document
  .getElementById("refreshMaintenanceBtn")
  .addEventListener("click", refreshMaintenanceData);

// ------------------------------------
// SAVE MAINTENANCE RECORD
// ------------------------------------

document
  .getElementById("maintenanceForm")
  .addEventListener("submit", async (event) => {
    event.preventDefault();

    const message = document.getElementById("maintenanceMessage");

    message.style.color = "#b45309";
    message.textContent = "Saving maintenance record...";

    const assetID = Number(document.getElementById("maintenanceAssetId").value);

    const problem = document.getElementById("maintenanceProblem").value.trim();

    const repairDate = document.getElementById("repairDate").value;

    const repairCost = Number(document.getElementById("repairCost").value);

    const technician = document.getElementById("technician").value.trim();

    const repairStatus = document.getElementById("repairStatus").value;

    const maintenanceData = {
      assetID: assetID,

      problem: problem,

      repairDate: formatDateForBackend(repairDate),

      repairCost: repairCost,

      technician: technician,

      repairStatus: repairStatus,
    };

    try {
      const response = await fetch(`${API_BASE_URL}/api/maintenance`, {
        method: "POST",

        headers: {
          "Content-Type": "application/json",
        },

        body: JSON.stringify(maintenanceData),
      });

      const result = await response.json();

      if (!response.ok) {
        throw new Error(result.message || "Could not save maintenance record.");
      }

      // Reload data from C++ backend
      await refreshMaintenanceData();

      // Reset the form
      document.getElementById("maintenanceForm").reset();

      setDefaultOperationDates();

      message.style.color = "#15803d";

      message.textContent = `Maintenance record saved successfully for Asset ${result.assetID}.`;

      alert(
        "Maintenance record saved successfully!\n\n" +
          "Asset ID: " +
          result.assetID +
          "\n" +
          "Repair Status: " +
          result.repairStatus,
      );
    } catch (error) {
      message.style.color = "#dc2626";

      message.textContent = error.message;

      console.error("Maintenance submission error:", error);
    }
  });

function setDefaultOperationDates() {
  const now = new Date();

  const localDate = [
    now.getFullYear(),
    String(now.getMonth() + 1).padStart(2, "0"),
    String(now.getDate()).padStart(2, "0"),
  ].join("-");

  document.getElementById("allocateDate").value = localDate;

  document.getElementById("transferDate").value = localDate;

  document.getElementById("repairDate").value = localDate;
}

setDefaultOperationDates();

// ------------------------------------
// LOAD WARRANTY ALERTS
// ------------------------------------

async function loadWarrantyAlerts() {
  const response = await fetch(`${API_BASE_URL}/api/warranty-alerts`);

  if (!response.ok) {
    throw new Error("Could not load warranty alerts.");
  }

  const records = await response.json();

  if (!Array.isArray(records)) {
    throw new Error("Invalid warranty data received.");
  }

  warrantyRecords.splice(0, warrantyRecords.length, ...records);

  return true;
}

// ------------------------------------
// WARRANTY SUMMARY
// ------------------------------------

function updateWarrantySummary() {
  const expired = warrantyRecords.filter(
    (record) => record.warrantyStatus === "Expired",
  );

  const within30 = warrantyRecords.filter(
    (record) => record.warrantyStatus === "Within 30 Days",
  );

  const within60 = warrantyRecords.filter(
    (record) => record.warrantyStatus === "Within 60 Days",
  );

  document.getElementById("totalWarrantyAlerts").textContent =
    warrantyRecords.length;

  document.getElementById("expiredWarranties").textContent = expired.length;

  document.getElementById("warrantyWithin30").textContent = within30.length;

  document.getElementById("warrantyWithin60").textContent = within60.length;

  // Update the warranty count on the main dashboard
  document.getElementById("warrantyAlerts").textContent =
    warrantyRecords.length;
}

// ------------------------------------
// DISPLAY WARRANTY ALERTS
// ------------------------------------

function displayWarrantyAlerts() {
  const tableBody = document.getElementById("warrantyTableBody");

  const search = document
    .getElementById("warrantySearch")
    .value.trim()
    .toLowerCase();

  const statusFilter = document.getElementById("warrantyFilter").value;

  const filteredRecords = warrantyRecords.filter((record) => {
    const searchableText = [
      record.assetID,
      record.assetType,
      record.brand,
      record.model,
      record.serialNumber,
      record.department,
      record.location,
      record.warrantyEndDate,
      record.warrantyStatus,
    ]
      .join(" ")
      .toLowerCase();

    return (
      searchableText.includes(search) &&
      (statusFilter === "" || record.warrantyStatus === statusFilter)
    );
  });

  // Show most urgent alerts first
  filteredRecords.sort((a, b) => a.daysRemaining - b.daysRemaining);

  if (filteredRecords.length === 0) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="7">
                    No matching warranty alerts found.
                </td>
            </tr>
        `;
  } else {
    tableBody.innerHTML = filteredRecords
      .map((record) => {
        let statusClass = "";
        let daysClass = "";
        let daysText = "";

        if (record.daysRemaining < 0) {
          statusClass = "badge-expired";
          daysClass = "days-expired";

          daysText = `${Math.abs(record.daysRemaining)} days overdue`;
        } else if (record.daysRemaining <= 30) {
          statusClass = "badge-within-30";
          daysClass = "days-within-30";

          daysText =
            record.daysRemaining === 0
              ? "Expires today"
              : `${record.daysRemaining} days left`;
        } else {
          statusClass = "badge-within-60";
          daysClass = "days-within-60";

          daysText = `${record.daysRemaining} days left`;
        }

        return `
                <tr>

                    <td class="asset-id">
                        ${escapeHTML(record.assetID)}
                    </td>

                    <td>
                        <div class="asset-name">
                            ${escapeHTML(record.brand)}
                        </div>

                        <div class="asset-subtitle">
                            ${escapeHTML(record.assetType)}
                            ·
                            ${escapeHTML(record.model)}
                        </div>
                    </td>

                    <td>
                        ${escapeHTML(record.department)}
                    </td>

                    <td>
                        ${escapeHTML(record.location)}
                    </td>

                    <td>
                        ${escapeHTML(record.warrantyEndDate)}
                    </td>

                    <td class="${daysClass}">
                        ${escapeHTML(daysText)}
                    </td>

                    <td>
                        <span class="status-badge ${statusClass}">
                            ${escapeHTML(record.warrantyStatus)}
                        </span>
                    </td>

                </tr>
            `;
      })
      .join("");
  }

  document.getElementById("warrantyRecordCount").textContent =
    `Showing ${filteredRecords.length} of ${warrantyRecords.length} alerts`;
}

// ------------------------------------
// REFRESH WARRANTY PAGE
// ------------------------------------

async function refreshWarrantyData() {
  const tableBody = document.getElementById("warrantyTableBody");

  try {
    await loadWarrantyAlerts();

    updateWarrantySummary();

    displayWarrantyAlerts();
  } catch (error) {
    tableBody.innerHTML = `
            <tr>
                <td colspan="7">
                    ${escapeHTML(error.message)}
                </td>
            </tr>
        `;

    console.error("Warranty refresh error:", error);
  }
}

// Search warranty records
document
  .getElementById("warrantySearch")
  .addEventListener("input", displayWarrantyAlerts);

// Filter warranty records
document
  .getElementById("warrantyFilter")
  .addEventListener("change", displayWarrantyAlerts);

// Refresh button
document
  .getElementById("refreshWarrantyBtn")
  .addEventListener("click", refreshWarrantyData);

// ------------------------------------
// LOAD REPORTS FROM C++ BACKEND
// ------------------------------------

let reportsData = null;

async function loadReportsData() {
  const response = await fetch(`${API_BASE_URL}/api/reports`);

  if (!response.ok) {
    throw new Error("Could not load reports.");
  }

  const data = await response.json();

  if (!data.summary) {
    throw new Error("Invalid report data received.");
  }

  reportsData = data;

  return true;
}

// ------------------------------------
// RENDER REPORT BREAKDOWN
// ------------------------------------

function renderReportBreakdown(containerID, records) {
  const container = document.getElementById(containerID);

  if (!records || records.length === 0) {
    container.textContent = "No data available.";

    return;
  }

  const maxCount = Math.max(...records.map((item) => Number(item.count)), 1);

  container.innerHTML = records
    .map((item) => {
      const count = Number(item.count) || 0;

      const percentage = Math.max(0, Math.min(100, (count / maxCount) * 100));

      return `
            <div class="report-bar-row">

                <span class="report-bar-label">
                    ${escapeHTML(item.label)}
                </span>

                <div class="report-bar-track">
                    <div
                        class="report-bar-fill"
                        style="width: ${percentage}%"
                    ></div>
                </div>

                <span class="report-bar-value">
                    ${count}
                </span>

            </div>
        `;
    })
    .join("");
}

// ------------------------------------
// UPDATE REPORT SUMMARY
// ------------------------------------

function updateReportsSummary() {

    const summary = reportsData.summary;

    document.getElementById("reportTotalAssets").textContent =
        summary.totalAssets;

    document.getElementById("reportInUse").textContent =
        summary.inUse;

    document.getElementById("reportInStock").textContent =
        summary.inStock;

    document.getElementById("reportUnderRepair").textContent =
        summary.underRepair;

    document.getElementById("reportRetired").textContent =
        summary.retired;

    document.getElementById("reportMaintenanceCost").textContent =
        formatCurrency(summary.totalMaintenanceCost);


    // Render the four breakdown reports
    renderReportBreakdown(
        "reportStatusBreakdown",
        reportsData.statusBreakdown
    );

    renderReportBreakdown(
        "reportCategoryBreakdown",
        reportsData.categoryBreakdown
    );

    renderReportBreakdown(
        "reportDepartmentBreakdown",
        reportsData.departmentBreakdown
    );

    renderReportBreakdown(
        "reportAgeBreakdown",
        reportsData.ageBreakdown
    );
}

// ------------------------------------
// MAINTENANCE EXPENDITURE REPORT
// ------------------------------------

function displayReportMaintenance() {

    const tableBody =
        document.getElementById("reportMaintenanceTable");

    const records = reportsData.maintenanceByAsset || [];


    if (records.length === 0) {

        tableBody.innerHTML = `
            <tr>
                <td colspan="5">No maintenance expenditure recorded.</td>
            </tr>
        `;

        return;
    }


    tableBody.innerHTML = records.map(record => {

        return `
            <tr>

                <td class="asset-id">
                    ${escapeHTML(record.assetID)}
                </td>

                <td>
                    <div class="asset-name">
                        ${escapeHTML(record.brand)}
                    </div>

                    <div class="asset-subtitle">
                        ${escapeHTML(record.model)}
                    </div>
                </td>

                <td>
                    ${escapeHTML(record.recordCount)}
                </td>

                <td>
                    ${escapeHTML(formatCurrency(record.purchaseCost))}
                </td>

                <td>
                    <strong>
                        ${escapeHTML(formatCurrency(record.totalCost))}
                    </strong>
                </td>

            </tr>
        `;

    }).join("");
}

// ------------------------------------
// RETIRED ASSET REPORT
// ------------------------------------

function displayReportRetired() {

    const tableBody =
        document.getElementById("reportRetiredTable");

    const records = reportsData.retiredAssets || [];


    if (records.length === 0) {

        tableBody.innerHTML = `
            <tr>
                <td colspan="6">No retired assets found.</td>
            </tr>
        `;

        return;
    }


    tableBody.innerHTML = records.map(asset => {

        return `
            <tr>

                <td class="asset-id">
                    ${escapeHTML(asset.assetID)}
                </td>

                <td>
                    <div class="asset-name">
                        ${escapeHTML(asset.brand)}
                    </div>

                    <div class="asset-subtitle">
                        ${escapeHTML(asset.assetType)}
                        ·
                        ${escapeHTML(asset.model)}
                    </div>
                </td>

                <td>${escapeHTML(asset.department)}</td>

                <td>${escapeHTML(asset.location)}</td>

                <td>${escapeHTML(asset.purchaseDate)}</td>

                <td>
                    ${escapeHTML(formatCurrency(asset.purchaseCost))}
                </td>

            </tr>
        `;

    }).join("");
}

// ------------------------------------
// REPLACEMENT RECOMMENDATIONS
// ------------------------------------

function displayReplacementRecommendations() {

    const tableBody =
        document.getElementById("reportReplacementTable");

    const records =
        reportsData.replacementRecommendations || [];


    if (records.length === 0) {

        tableBody.innerHTML = `
            <tr>
                <td colspan="6">
                    No assets currently meet the replacement review criteria.
                </td>
            </tr>
        `;

        return;
    }


    tableBody.innerHTML = records.map(asset => {

        const reasons = (asset.reasons || [])
            .map(reason => escapeHTML(reason))
            .join("<br>");


        const ageText = asset.ageYears >= 0
            ? Number(asset.ageYears).toFixed(1)
            : "Unknown";


        return `
            <tr>

                <td class="asset-id">
                    ${escapeHTML(asset.assetID)}
                </td>

                <td>
                    <div class="asset-name">
                        ${escapeHTML(asset.brand)}
                    </div>

                    <div class="asset-subtitle">
                        ${escapeHTML(asset.model)}
                    </div>
                </td>

                <td>
                    ${escapeHTML(asset.status)}
                </td>

                <td>
                    ${escapeHTML(ageText)}
                </td>

                <td>
                    ${escapeHTML(formatCurrency(asset.maintenanceCost))}
                </td>

                <td>
                    <span class="report-recommendation">
                        Review Required
                    </span>

                    <div class="report-reasons">
                        ${reasons}
                    </div>
                </td>

            </tr>
        `;

    }).join("");
}

// ------------------------------------
// REFRESH REPORTS PAGE
// ------------------------------------

async function refreshReportsData() {

    const message =
        document.getElementById("reportsMessage");

    message.style.color = "#b45309";


    try {

        await loadReportsData();

        updateReportsSummary();

        displayReportMaintenance();

        displayReportRetired();

        displayReplacementRecommendations();

        message.textContent = "";

    }
    catch (error) {

        message.style.color = "#dc2626";
        message.textContent = error.message;

        console.error("Reports error:", error);
    }
}


// Refresh report button
document.getElementById("refreshReportsBtn")
    .addEventListener("click", refreshReportsData);

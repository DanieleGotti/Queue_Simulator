import numpy as np
import matplotlib.pyplot as plt

def plot_single_system(rho, delays, errors, title, label, fmt):
    plt.figure(figsize=(8, 5))
    plt.errorbar(rho, delays, yerr=errors, fmt=fmt, capsize=5, label=label)
    plt.xlabel('ρ o Probabilità di errore')
    plt.ylabel('Ritardo medio (s)')
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plot_all_individual_systems():
    # ===== S0 =====
    rho_S0    = np.array([0.2, 0.4, 0.6, 0.8, 0.99])
    delays_S0 = np.array([0.16, 0.73, 1.44, 3.76, 12.55])
    error_S0  = np.array([0.16, 0.684, 0.760, 3.79, 7.98])
    plot_single_system(rho_S0, delays_S0, error_S0, "Sistema S0: M/M/1 base", "S0", 'o-')

    # ===== S1 =====
    rho_S1    = np.array([0.2, 0.4, 0.6, 0.8, 0.99])
    delays_S1 = np.array([2.1, 2.08, 2.09, 2.03, 2.13])
    error_S1  = np.array([0.548, 0.215, 0.366, 0.265, 0.248])
    plot_single_system(rho_S1, delays_S1, error_S1, "Sistema S1: Stop&Wait, ACK lento", "S1", 's--')

    # ===== S2 =====
    rho_S2    = np.array([0.2, 0.4, 0.6, 0.8, 0.99])
    delays_S2 = np.array([5.54, 2.77, 2.06, 1.63, 1.27])
    error_S2  = np.array([1.25, 0.279, 0.379, 0.223, 0.172])
    plot_single_system(rho_S2, delays_S2, error_S2, "Sistema S2: ρ costante = 1", "S2", '^:')

    # ===== S3 =====
    prob_S3   = np.array([0.2, 0.4, 0.6, 0.8, 0.99])
    delays_S3 = np.array([1.85, 2.22, 2.31, 7.07, 115.72])
    error_S3  = np.array([0.222, 0.546, 0.745, 3.14, 111.10])
    plot_single_system(prob_S3, delays_S3, error_S3, "Sistema S3: errore crescente", "S3", 'd-.')

# Esegui la funzione
plot_all_individual_systems()

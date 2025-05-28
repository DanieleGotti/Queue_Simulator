import numpy as np
import matplotlib.pyplot as plt

def plot_delay_with_ci(lambdas, mu, delays, ci_lower, ci_upper,
                       xlabel='ρ', ylabel='Ritardo medio (s)',
                       title='Ritardo medio vs carico',
                       marker='o-', capsize=5):
    """
    Grafico 1: Ritardo medio con intervalli di confidenza
    Input:
      lambdas   : vettore di tassi di arrivo λ
      mu         : servizio μ (scalare o vettore della stessa lunghezza)
      delays     : vettore dei ritardi medi osservati
      ci_lower   : vettore semilarghezze inferiore (delay - lower bound)
      ci_upper   : vettore semilarghezze superiore (upper bound - delay)
    """
    rho = np.array(lambdas) / np.array(mu)
    yerr = np.vstack([ci_lower, ci_upper])
    plt.figure(figsize=(8,5))
    plt.errorbar(rho, delays, yerr=yerr, fmt=marker, capsize=capsize, lw=1.5)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_throughput(lambdas, mu, delays,
                    xlabel='ρ', ylabel='Throughput (pkt/s)',
                    title='Throughput vs carico',
                    marker='s-'):
    """
    Grafico 2: Throughput effettivo = 1 / (ritardo medio)
    Input:
      lambdas : vettore di λ (solo per calcolare ρ sull'asse X)
      mu      : servizio μ (scalare o vettore)
      delays  : vettore dei ritardi medi osservati
    """
    rho = np.array(lambdas) / np.array(mu)
    throughput = 1.0 / np.array(delays)
    plt.figure(figsize=(8,5))
    plt.plot(rho, throughput, marker, lw=1.5)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_delay_std(lambdas, mu, std_delays,
                   xlabel='ρ', ylabel='Deviazione standard ritardo (s)',
                   title='Jitter vs carico',
                   marker='d-'):
    """
    Grafico 3: Deviazione standard del ritardo (jitter)
    Input:
      lambdas   : vettore di λ
      mu        : servizio μ
      std_delays: vettore delle deviazioni standard dei ritardi
    """
    rho = np.array(lambdas) / np.array(mu)
    plt.figure(figsize=(8,5))
    plt.plot(rho, std_delays, marker, lw=1.5)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_relative_vs_mm1(lambdas, mu, delays,
                         xlabel='ρ', ylabel='Diff. relativa',
                         title='(W_SW - W_MM1) / W_MM1',
                         marker='^-'):
    """
    Grafico 4: Differenza relativa rispetto a M/M/1
    Input:
      lambdas: vettore di λ
      mu      : servizio μ
      delays  : vettore W_SW (ritardi medi Stop-&-Wait)
    """
    rho = np.array(lambdas) / np.array(mu)
    W_mm1 = 1.0 / (np.array(mu) - np.array(lambdas))
    rel_diff = (np.array(delays) - W_mm1) / W_mm1
    plt.figure(figsize=(8,5))
    plt.plot(rho, rel_diff, marker, lw=1.5)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_utilizations(lambdas, mu_data, mu_ack,
                      xlabel='ρ', ylabel='Utilizzazione',
                      title='Utilizzazione canali vs carico',
                      markers=('o-', 's-')):
    """
    Grafico 5: Utilizzazione del canale dati e ACK nello Stop-&-Wait
    Input:
      lambdas : vettore di λ
      mu_data : servizio dati μ_data
      mu_ack  : servizio ACK μ_ack
    """
    lambdas = np.array(lambdas)
    mu_data = np.array(mu_data)
    mu_ack  = np.array(mu_ack)
    rho = lambdas / (1.0 / (1.0/mu_data + 1.0/mu_ack))
    # tempo medio servizio dati e ACK
    S_data = 1.0 / mu_data
    S_ack  = 1.0 / mu_ack
    T_cycle = S_data + S_ack
    util_data = (lambdas * S_data) / 1.0
    util_ack  = (lambdas * S_ack) / 1.0
    plt.figure(figsize=(8,5))
    plt.plot(rho, util_data, markers[0], lw=1.5, label='Dati')
    plt.plot(rho, util_ack,  markers[1], lw=1.5, label='ACK')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_delay_vs_error(p_errors, delays, ci_lower=None, ci_upper=None,
                        xlabel='Probability of error',
                        ylabel='Average delay (s)',
                        title='Average delay vs error probability',
                        marker='o-', capsize=5):
    """
    Grafico: ritardo medio vs probabilità di errore nel protocollo Stop-&-Wait.
    
    Parameters
    ----------
    p_errors : array-like, shape (n,)
        Vettore delle probabilità di errore (asse X).
    delays : array-like, shape (n,)
        Vettore dei ritardi medi corrispondenti (asse Y).
    ci_lower : array-like, shape (n,), optional
        Semilarghezze inferiore delle barre di confidenza.
    ci_upper : array-like, shape (n,), optional
        Semilarghezze superiore delle barre di confidenza.
    """
    plt.figure(figsize=(8, 5))
    if ci_lower is not None and ci_upper is not None:
        yerr = np.vstack([ci_lower, ci_upper])
        plt.errorbar(p_errors, delays, yerr=yerr, fmt=marker, capsize=capsize, lw=1.5)
    else:
        plt.plot(p_errors, delays, marker, lw=1.5)
    
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_delay_histogram(delays, bins=30, title='Distribuzione dei ritardi'):
    """
    Istogramma della distribuzione dei ritardi
    Input:
      delays: vettore dei ritardi osservati
      bins: numero di bin dell'istogramma
    """
    plt.figure(figsize=(8,5))
    plt.hist(delays, bins=bins, alpha=0.7, color='skyblue', edgecolor='k')
    plt.xlabel('Ritardo (s)')
    plt.ylabel('Frequenza')
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_max_buffer_vs_rho(lambdas, mu, max_buffers):
    """
    Occupazione massima buffer vs carico
    Input:
      lambdas: vettore di λ
      mu: vettore o scalare μ
      max_buffers: vettore delle max occupazioni buffer
    """
    rho = np.array(lambdas) / np.array(mu)
    plt.figure(figsize=(8,5))
    plt.plot(rho, max_buffers, 'o-', lw=1.5)
    plt.xlabel('ρ')
    plt.ylabel('Max buffer occupancy')
    plt.title('Occupazione massima buffer vs carico')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_sim_vs_theory(rho, sim_delay, theo_delay):
    """
    Confronto tra ritardo medio simulato e teorico
    Input:
      rho: vettore dei carichi
      sim_delay: ritardi medi simulati
      theo_delay: ritardi medi teorici
    """
    plt.figure(figsize=(8,5))
    plt.plot(rho, sim_delay, 'o-', label='Simulato')
    plt.plot(rho, theo_delay, 's--', label='Teorico')
    plt.xlabel('ρ')
    plt.ylabel('Ritardo medio (s)')
    plt.title('Ritardo medio: simulato vs teorico')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

def plot_delay_boxplot(delays_by_rho, rhos):
    """
    Boxplot dei ritardi per diversi carichi
    Input:
      delays_by_rho: lista di array di ritardi per ogni carico
      rhos: lista dei valori di carico (ρ)
    """
    plt.figure(figsize=(8,5))
    plt.boxplot(delays_by_rho, labels=[f'{r:.2f}' for r in rhos])
    plt.xlabel('ρ')
    plt.ylabel('Ritardo (s)')
    plt.title('Boxplot dei ritardi per diversi carichi')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Esempio dati
    lambdas = np.array([0.2, 0.4, 0.6, 0.8, 0.99])
    mu = 1.0
    delays = np.array([0.16, 0.73, 1.44, 3.70, 12.55])
    error = np.array([0.16, 0.684, 0.760, 3.79, 7.98])  # errore ±

    ci_lower = error
    ci_upper = error

    std_delays = np.array([0.02, 0.04, 0.06, 0.1, 0.2])
    mu_data = 1.0
    mu_ack = 1.0

    p_errors = np.linspace(0, 1, 11)
    mu_eff = 0.5
    delays_mean = 1.0 / (mu_eff * (1 - p_errors + 1e-6))

    plot_delay_with_ci(lambdas, mu, delays, ci_lower, ci_upper)
    plot_throughput(lambdas, mu, delays)
    plot_delay_std(lambdas, mu, std_delays)
    plot_relative_vs_mm1(lambdas, mu, delays)
    plot_utilizations(lambdas, mu_data, mu_ack)
    plot_delay_vs_error(p_errors, delays_mean)
    plot_delay_histogram(delays)
    plot_max_buffer_vs_rho(lambdas, mu, np.array([0.5, 0.7, 0.9]))
    plot_sim_vs_theory(np.array([0.2, 0.4, 0.6, 0.8, 0.99]), delays, 1.0 / (lambdas - lambdas + 1e-6))
    plot_delay_boxplot([delays, np.array([0.1, 0.2, 0.3, 0.4, 0.5])], np.array([0.2, 0.4, 0.6, 0.8, 0.99]))
